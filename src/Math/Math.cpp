#include "Math.h"
#include <chrono>
#include <queue>
#include "glm/vec3.hpp"
#include <iostream>
#include <mutex>
#include <thread>


glm::vec3 Math::CalculateBSplineSurfacePoint(const std::vector<std::vector<glm::vec3>>& controlPoints,
                                             const std::vector<float>& knotVectorU,
                                             const std::vector<float>& knotVectorV,
                                             float u, float v, int degreeU, int degreeV)
{
    int numControlPointsU = controlPoints.size();
    int numControlPointsV = controlPoints[0].size();

    glm::vec3 surfacePoint(0.0f);

    for (int i = 0; i < numControlPointsU; ++i) {
        for (int j = 0; j < numControlPointsV; ++j) {
    
            float basisU = BasisFunction(degreeU, knotVectorU, i, u);
            float basisV = BasisFunction(degreeV, knotVectorV, j, v);

            surfacePoint += basisU * basisV * controlPoints[i][j];
        }
    }

    return surfacePoint;
}

std::vector<float> Math::CalculateKnotVector(int numControlPoints, int degree)
{
    int totalKnots = numControlPoints + degree + 1;
    std::vector<float> knotVector;
	knotVector.reserve(totalKnots);

    for (int i = 0; i <= degree; ++i) {
        knotVector.emplace_back(0.f);
    }
    for (int i = degree + 1; i < numControlPoints; ++i) {
        knotVector.emplace_back(static_cast<float>(i - degree) / (numControlPoints - degree));
    }
    for (int i = numControlPoints; i < totalKnots; ++i) {
        knotVector.emplace_back(1.f);
    }

    return knotVector;
}

std::vector<std::vector<glm::vec3>> Math::Create2DVectorFrom1D(std::vector<glm::vec3> vector)
{
    auto [gridVector, numPrRow] = Math::CalculateChunks(vector,3.f);
    std::vector<std::vector<glm::vec3>> returnVector(gridVector.size()/numPrRow,std::vector<glm::vec3>(numPrRow));
    for(int i = 0;i<gridVector.size()/numPrRow;i++)
    {
	    for (int j = 0; j < numPrRow; ++j)
	    {
            returnVector[i][j] = gridVector[i * numPrRow + j];
	    }
    }
    return returnVector;
}

void UpdateProgressBar(int completedTasks, int totalTasks, std::mutex& progressMutex)
{
    int progressPercentage = (completedTasks * 100) / totalTasks;

    // Build progress bar string
    int barWidth = 50;
    int filled = (progressPercentage * barWidth) / 100;
    std::string bar(filled, '|');
    bar.append(barWidth - filled, ' ');

    // Print the progress bar
    {
        std::lock_guard<std::mutex> lock(progressMutex);
        std::cout << "\rProgress: [" << bar << "] " << progressPercentage << "%";
        std::cout.flush();
    }
}

std::vector<glm::vec3> Math::CalculateSurface(int samples, const std::vector<std::vector<glm::vec3>>& controlPoints)
{

    int numControlPointsU = controlPoints.size();    // Number of control points in U direction
    int numControlPointsV = controlPoints[0].size(); // Number of control points in V direction

    // Knot vectors for U and V directions
    std::vector<float> knotVectorU = CalculateKnotVector(numControlPointsU, 2);
    std::vector<float> knotVectorV = CalculateKnotVector(numControlPointsV, 2);

    float sampleStep = 1.0f / (samples - 1);

    // Prepare container for the resulting surface points
    std::vector<glm::vec3> surfacePoints(samples*samples);
    std::atomic<int> completed(0);
    int total = samples*samples;
    std::mutex progressMutex;

    int numThreads = std::thread::hardware_concurrency();
    std::cout << numThreads << '\n';
    std::vector<std::thread> threads(numThreads);
    for(int t = 0;t<numThreads;t++)
    {
        int start = samples / numThreads * t;
        int end = samples / numThreads * (t + 1);
        threads[t] = std::thread([sampleStep, &completed, total,&surfacePoints, &progressMutex, &controlPoints, &knotVectorU, &knotVectorV, start, end, samples,t]()
        {
            for (int i = start; i < end; ++i) {
                float u = i * sampleStep;
                for (int j = 0; j < samples; ++j) {
                    float v = j * sampleStep;

                    glm::vec3 surfacePoint = CalculateBSplineSurfacePoint(controlPoints, knotVectorU, knotVectorV, u, v, 2, 2);
                    surfacePoints[i*samples+j] = surfacePoint;
                    int local = ++completed;
                    if (local % 1000 == 0) {
                        UpdateProgressBar(completed, total, progressMutex);
                    }
                }
                
            }
        });
    }

    for(int i = 0; i<numThreads; i++)
    {
        if(threads[i].joinable())
			threads[i].join();
    }
	UpdateProgressBar(completed,total,progressMutex);

    return surfacePoints;
}


float Math::BasisFunction(int d, const std::vector<float>& knots, int i, float t) 
{
    if (d == 0 || i+d+1>=knots.size()) {
        return (t >= knots[i] && t < knots[i + 1]) ? 1.0f : 0.0f;
    }

    float leftWeight = 0.0f;
    if (knots[i + d] - knots[i] != 0.0f) 
    {
        leftWeight = (t - knots[i]) / (knots[i + d] - knots[i]);
    }
    float leftBasis = leftWeight * BasisFunction(d - 1, knots, i, t);

    float rightWeight = 0.0f;
    if (knots[i + d + 1] - knots[i + 1] != 0.0f) {
        rightWeight = (knots[i + d + 1] - t) / (knots[i + d + 1] - knots[i + 1]);
    }
    float rightBasis = rightWeight * BasisFunction(d - 1, knots, i + 1, t);

    return leftBasis + rightBasis;
}

std::pair<std::vector<glm::vec3>, int> Math::CalculateChunks(std::vector<glm::vec3>& pointCloud, float ChunkSize)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<glm::vec3> sortedPoints = pointCloud;
    std::ranges::sort(sortedPoints, [](const glm::vec3& v1, const glm::vec3& v2)
        {
            if (v1.z != v2.z)
                return v1.z < v2.z;
            return v1.x < v2.x;
        });

    float spacing = ChunkSize; 
    std::vector<glm::vec3> gridPoints;
    glm::vec3 firstPos = sortedPoints.front();
    glm::vec3 lastPos = sortedPoints.back();
    int numPerRow = 0;
    int numRows = 0;

    for (float z = firstPos.z; z <= lastPos.z; z += spacing)
    {
        int rowCount = 0;
        for (float x = firstPos.x; x <= lastPos.x; x += spacing)
        {
            gridPoints.emplace_back(x, 0.f, z);
            rowCount++;
        }
        numPerRow = rowCount; 
        numRows++;
    }

    std::vector<float> ySums(gridPoints.size(), 0.f);
    std::vector<int> yCounts(gridPoints.size(), 0);

    for (const auto& point : sortedPoints)
    {
        int cellX = static_cast<int>((point.x - firstPos.x) / spacing);
        int cellZ = static_cast<int>((point.z - firstPos.z) / spacing);

        if (cellX < 0 || cellX >= numPerRow || cellZ < 0 || cellZ >= numRows)
            continue;

        int cellIndex = cellZ * numPerRow + cellX;
        if (cellIndex < 0 || cellIndex >= gridPoints.size())
            continue;

        ySums[cellIndex] += point.y;
        yCounts[cellIndex]++;
    }

    for (size_t i = 0; i < gridPoints.size(); i++)
    {
        if (yCounts[i] > 0)
        {
            if(ySums[i])
            gridPoints[i].y = ySums[i] / yCounts[i];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time Elapsed: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms\n";

    return { gridPoints, numPerRow };
}
