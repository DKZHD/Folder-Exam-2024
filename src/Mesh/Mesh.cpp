#include "Mesh.h"
#include <iostream>
#include "../Engine.h"
#include "../Input/Input.h"
#include "../Loaders/fileLoader.h"
#include "../Math/Math.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"


Mesh::Mesh(MeshType type, glm::vec3 pos, glm::vec3 scale) : position(pos),scale(scale), radius(scale.x)
{
	if(type==MeshType::Cube)
	{
		vertices = {
		    glm::vec3(-0.5,-0.5,0.5),
		    glm::vec3(0.5,-0.5,0.5),
		    glm::vec3(0.5,0.5,0.5),
		    glm::vec3(-0.5,0.5,0.5),
		    glm::vec3(-0.5,-0.5,-0.5),
		    glm::vec3(0.5,-0.5,-0.5),
		    glm::vec3(0.5,0.5,-0.5),
		    glm::vec3(-0.5,0.5,-0.5)
		};

		indices = {
			0,1,2,2,3,0,
			0,3,7,7,4,0,
			3,2,6,6,7,3,
			1,5,6,6,2,1,
			0,4,5,5,1,0,
			4,5,6,6,7,4
		};
	}
	if(type==MeshType::Sphere)
	{
		vertices.emplace_back(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
		vertices.emplace_back(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		vertices.emplace_back(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f));
		vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
		vertices.emplace_back(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, -1.f));
		for(Vertex& vert : vertices)
		{
			vert.color = glm::vec3(1.f, 0.f, 0.f);
		}
		Subdivide(0, 1, 2, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(0, 2, 3, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(0, 3, 4, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(0, 4, 1, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(5, 2, 1, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(5, 3, 2, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(5, 4, 3, 3, glm::vec3(1.f,0.f,0.f));
		Subdivide(5, 1, 4, 3, glm::vec3(1.f,0.f,0.f));
	}
	BindBuffer();
}

void Mesh::Subdivide(int index1, int index2, int index3, int i, glm::vec3 color)
{
	if (i > 0) {
		glm::vec3 pos1 = glm::normalize(vertices[index1].pos + vertices[index2].pos);
		int t_index1 = vertices.size();
		vertices.emplace_back(pos1, pos1);
		vertices.back().color = color;

		glm::vec3 pos2 = glm::normalize(vertices[index1].pos + vertices[index3].pos);
		int t_index2 = vertices.size();
		vertices.emplace_back(pos2, pos2);
		vertices.back().color = color;


		glm::vec3 pos3 = glm::normalize(vertices[index3].pos + vertices[index2].pos);
		int t_index3 = vertices.size();
		vertices.emplace_back(pos3, pos3);
		vertices.back().color = color;


		Subdivide(index1, t_index1, t_index2, i - 1,color);
		Subdivide(index3, t_index2, t_index3, i - 1,color);
		Subdivide(index2, t_index3, t_index1, i - 1,color);
		Subdivide(t_index3, t_index2, t_index1, i - 1,color);
	}
	else {
		indices.emplace_back(index1);
		indices.emplace_back(index2);
		indices.emplace_back(index3);
	}
}

void Mesh::UpdatePosition(glm::vec3 pos)
{
	position += pos;
}

void Mesh::SetPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3& Mesh::GetPosition() const
{
	return const_cast<glm::vec3&>(position);
}

void Mesh::UpdateVelocity(glm::vec3 vel)
{
	velocity += vel;
}

bool Mesh::checkCollision(Mesh& mesh)
{
	float dx = mesh.GetPosition().x - GetPosition().x;
	float dy = mesh.GetPosition().y - GetPosition().y;
	float dz = mesh.GetPosition().z - GetPosition().z;
	float l = sqrtf(dx * dx + dy * dy+dz*dz);
	float d = radius + mesh.radius;
	if (l >= d)
		return false;

	float detM = mass + mesh.mass;
	float numM1 = 2 * mass;
	float numM2 = 2 * mesh.mass;

	float v1Num = glm::dot(velocity - mesh.velocity, GetPosition() - mesh.GetPosition());
	float v2Num = glm::dot(mesh.velocity - velocity, mesh.GetPosition() - GetPosition());

	float detV1 = glm::distance(GetPosition(), mesh.GetPosition());
	float detV2 = glm::distance(mesh.GetPosition(), GetPosition());

	glm::vec3 v1 = velocity - (numM2 / detM) * (v1Num / (detV1 * detV1)) * (GetPosition() - mesh.GetPosition());
	glm::vec3 v2 = mesh.velocity - (numM1 / detM) * (v2Num / (detV2 * detV2)) * (mesh.GetPosition() - GetPosition());

	velocity = v1;
	mesh.velocity = v2;

	glm::vec3 collisionV1 = glm::normalize(mesh.GetPosition() - GetPosition());

	position = position + collisionV1 * ((l - d) / 2.f);
	mesh.position = mesh.position - collisionV1 * ((l - d) / 2.f);

	return true;
}

void Mesh::Update(float deltaTime, float gravity)
{
	velocity.y += gravity * deltaTime;
	MathMesh& terrain = Engine::GetInstance().scene.GetTerrain();
	float height = terrain.GetHeightAtPosition(position);
	if(position.y-radius<height)
	{
		velocity.y = 0.f;
		position.y = height + radius;
		glm::vec3 surfaceNormal = terrain.GetNormalAtPosition(position);
		glm::vec3 G = glm::vec3(0.f,-gravity*mass,0.f);

		glm::vec3 a = -gravity * glm::vec3(surfaceNormal.x*surfaceNormal.y,surfaceNormal.y*surfaceNormal.y-1.f,surfaceNormal.z*surfaceNormal.y);
		float frictionCoefficient = terrain.GetFrictionCoEfficient(position);
		if (glm::length(velocity) > 0.0f)
		{
			glm::vec3 frictionAcceleration = -frictionCoefficient * glm::length(velocity) / deltaTime * glm::normalize(velocity);
			a += frictionAcceleration;
		}

		velocity += a * deltaTime;

		if (glm::length(glm::vec3(velocity.x, 0.f, velocity.z)) < 0.01)
		{
			velocity = glm::vec3(0.f);
		}
	}
	if (glm::length(glm::vec3(velocity.x, 0.f, velocity.z)) > 0.0f)
	{
		glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), velocity));
		float rotationAmount = glm::length(glm::vec3(velocity.x, 0.f, velocity.z)) * deltaTime / radius;
		glm::mat4 incrementalRotation = glm::rotate(glm::mat4(1.0f), rotationAmount, rotationAxis);
		rotationMatrix = incrementalRotation * rotationMatrix;
	}
	
	position += velocity * deltaTime;
}


void Mesh::Render(uint32_t program) const
{
	glBindVertexArray(vao);
	glm::mat4 model(1.f);
	model = glm::translate(model, position);
	model = model * rotationMatrix;
	model = glm::scale(model,scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, &model[0][0]);
	glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(indices.size()),GL_UNSIGNED_INT,nullptr);

	if(trackingPoints.size()>=4)
	{
		model = glm::mat4(1.f);
		glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, &model[0][0]);

		glBindVertexArray(tracking_vao);
		glDrawArrays(GL_LINE_STRIP, 0, trackingPoints.size());
	}
		
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "Error: " << std::to_string(error) << '\n';
	}
}

void Mesh::TrackBall(float deltaTime)
{
	constexpr int MAX_TRACKING_POINTS = 100;
	if(timer<0.1f)
	{
		timer += deltaTime;
		return;
	}
	timer = 0.f;
	if (controlPoints.size() >= MAX_TRACKING_POINTS) 
	{
		controlPoints.erase(controlPoints.begin());
	}
	controlPoints.emplace_back(position);
	trackingPoints.clear();

	int degree = 2;
	int numControlPoints = controlPoints.size();
	std::vector<float> knotVector = Math::CalculateKnotVector(numControlPoints, degree);

	for (float u = 0; u <= 1.f; u += 0.01f) 
	{
		glm::vec3 surfacePoint(0.f);
		for (int i = 0; i < numControlPoints; i++) 
		{
			float basisValue = Math::BasisFunction(degree, knotVector, i, u);
			surfacePoint += basisValue * controlPoints[i].pos;
		}
		trackingPoints.emplace_back(surfacePoint);
	}
	glBindBuffer(GL_ARRAY_BUFFER, tracking_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * trackingPoints.size(), trackingPoints.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::BindBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &tracking_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1,&ebo);
	glGenBuffers(1, &tracking_vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,color));
	glBindVertexArray(0);

	glBindVertexArray(tracking_vao);
	glBindBuffer(GL_ARRAY_BUFFER, tracking_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1001, trackingPoints.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glBindVertexArray(0);
}

MathMesh::MathMesh(std::vector<glm::vec3> v, int prRow, bool point_cloud) : numPrRow(prRow), position(glm::vec3(0.f))
{
	vertices.assign(v.begin(), v.end());
	for(Vertex& vert : vertices)
	{
		vert.color = glm::vec3(0.f,0.8f,0.f);
	}
	if(!point_cloud)
	{
		for (int i = 0; i < vertices.size()/numPrRow-1; i++)
		{
			for (int j = 0; j < numPrRow-1; j++)
			{
				if (j + 1 >= numPrRow || i * numPrRow > vertices.size())
					continue;
				if (vertices.size()/numPrRow-200<i)
				{
					vertices[i * numPrRow + j].color = glm::vec3(1.f, 0.f, 0.f);
					vertices[i * numPrRow + j + 1].color = glm::vec3(1.f, 0.f, 0.f);
					vertices[i * numPrRow + numPrRow + j].color = glm::vec3(1.f, 0.f, 0.f);
					vertices[i * numPrRow + numPrRow + j + 1].color = glm::vec3(1.f, 0.f, 0.f);
				}
				indices.emplace_back(i * numPrRow + j);
				indices.emplace_back(i * numPrRow + j + 1);
				indices.emplace_back(i * numPrRow + numPrRow + j + 1);
				indices.emplace_back(i * numPrRow + numPrRow + j + 1);
				indices.emplace_back(i * numPrRow + numPrRow + j);
				indices.emplace_back(i * numPrRow + j);

				glm::vec3 normal1 = glm::cross(vertices[i * numPrRow + j + 1].pos - vertices[i * numPrRow + j].pos, 
					vertices[i * numPrRow + numPrRow + j + 1].pos - vertices[i * numPrRow + j].pos);
				vertices[i * numPrRow + j].normal += normal1;
				vertices[i * numPrRow + j + 1].normal += normal1;
				vertices[i * numPrRow + numPrRow + j + 1].normal += normal1;

				glm::vec3 normal2 = glm::cross(vertices[i * numPrRow + numPrRow + j].pos - vertices[i * numPrRow + numPrRow + j + 1].pos, 
					vertices[i * numPrRow + j].pos - vertices[i * numPrRow + numPrRow + j + 1].pos);
				vertices[i * numPrRow + numPrRow + j + 1].normal += normal2;
				vertices[i * numPrRow + numPrRow + j].normal += normal2;
				vertices[i * numPrRow + j].normal += normal2;
			}

		}
	}
	BindBuffer();
}

void MathMesh::BindBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glBindVertexArray(0);
}

void MathMesh::Render(uint32_t program, bool point_cloud) const
{
	glBindVertexArray(vao);
	glm::mat4 model(1.f);
	model = glm::translate(model, position);
	glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, &model[0][0]);
	if(Input::Keyboard::pointCloud||point_cloud)
		glDrawArrays(GL_POINTS,0,vertices.size());
	else 
		glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,nullptr);
	
	GLenum error = glGetError();
	if(error!=GL_NO_ERROR)
	{
		std::cout << "Error: " << std::to_string(error) << '\n';
	}
}

float MathMesh::GetHeightAtPosition(glm::vec3 pos)
{
	if (pos.x<vertices.front().pos.x || pos.x>vertices.back().pos.x || pos.z>vertices.back().pos.z || pos.z<vertices.front().pos.z)
 		return -FLT_MAX;

	std::tuple<unsigned, unsigned, unsigned> indexes = GetTriangleIndicesAtPositon(pos);
	float height = BarycentricCalculation(std::get<0>(indexes), std::get<1>(indexes), std::get<2>(indexes), pos);
	return height;
}

glm::vec3 MathMesh::BarycentricCalculationVec(unsigned index0, unsigned index1, unsigned index2, glm::vec3 pos)
{
	float determinant = (vertices[index1].pos.z - vertices[index2].pos.z) * (vertices[index0].pos.x - vertices[index2].pos.x) + (vertices[index2].pos.x - vertices[index1].pos.x) * (vertices[index0].pos.z - vertices[index2].pos.z);
	float lambda1 = ((vertices[index1].pos.z - vertices[index2].pos.z) * (pos.x - vertices[index2].pos.x) + (vertices[index2].pos.x - vertices[index1].pos.x) * (pos.z - vertices[index2].pos.z)) / determinant;
	float lambda2 = ((vertices[index2].pos.z - vertices[index0].pos.z) * (pos.x - vertices[index2].pos.x) + (vertices[index0].pos.x - vertices[index2].pos.x) * (pos.z - vertices[index2].pos.z)) / determinant;
	float lambda3 = ((vertices[index0].pos.z - vertices[index1].pos.z) * (pos.x - vertices[index0].pos.x) + (vertices[index1].pos.x - vertices[index0].pos.x) * (pos.z - vertices[index0].pos.z)) / determinant;

	return { lambda1,lambda2,lambda3 };
}

float MathMesh::BarycentricCalculation(unsigned index0, unsigned index1, unsigned index2, glm::vec3 pos)
{
	glm::vec3 bary = BarycentricCalculationVec(index0,index1,index2,pos);
	return bary.x * vertices[index0].pos.y + bary.y * vertices[index1].pos.y + bary.z * vertices[index2].pos.y;
}

std::tuple<unsigned, unsigned, unsigned> MathMesh::GetTriangleIndicesAtPositon(glm::vec3 pos)
{
	int gridX = -1, gridZ = -1;
	for (int x = 0; x < numPrRow - 1; ++x) {
		if (vertices[x].pos.x <= pos.x && vertices[x + 1].pos.x > pos.x)
		{
			gridX = x;
			break;
		}
	}

	for (int z = 0; z < vertices.size() / numPrRow - 1; ++z)
	{
		if (vertices[z * numPrRow].pos.z <= pos.z && vertices[(z + 1) * numPrRow].pos.z > pos.z) {
			gridZ = z;
			break;
		}
	}

	// Identify the vertices of the grid cell
	unsigned int topLeft = gridZ * numPrRow + gridX;
	unsigned int topRight = topLeft + 1;
	unsigned int bottomLeft = (gridZ + 1) * numPrRow + gridX;
	unsigned int bottomRight = bottomLeft + 1;

	// Determine which triangle: lower-left or upper-right
	if (glm::length(BarycentricCalculationVec(topLeft,topRight,bottomRight,pos)) <= 1.0f) {
		return { topLeft, topRight, bottomRight };
	}
	else {
		return { topLeft, bottomRight, bottomLeft };
	}
}

glm::vec3 MathMesh::GetNormalAtPosition(glm::vec3 pos)
{
	glm::vec3 test = vertices.back().pos;
	if (pos.x<vertices.front().pos.x || pos.x>vertices.back().pos.x || pos.z > vertices.back().pos.z || pos.z < vertices.front().pos.z)
		return glm::vec3(-FLT_MAX);

	std::tuple<unsigned, unsigned, unsigned> indexes = GetTriangleIndicesAtPositon(pos);
	glm::vec3 barycentric = BarycentricCalculationVec(std::get<0>(indexes), std::get<1>(indexes), std::get<2>(indexes), pos);
	glm::vec3 normal = glm::normalize(barycentric.x * vertices[std::get<0>(indexes)].normal + 
		barycentric.y * vertices[std::get<1>(indexes)].normal + barycentric.z * vertices[std::get<2>(indexes)].normal);

	return normal;
}

float MathMesh::GetFrictionCoEfficient(glm::vec3 pos)
{
	if (pos.x<vertices.front().pos.x || pos.x>vertices.back().pos.x || pos.z > vertices.back().pos.z || pos.z < vertices.front().pos.z)
		return -FLT_MAX;
	std::tuple<unsigned, unsigned, unsigned> indexes = GetTriangleIndicesAtPositon(pos);
	glm::vec3 friction = glm::normalize(vertices[std::get<0>(indexes)].color + vertices[std::get<1>(indexes)].color + vertices[std::get<2>(indexes)].color);
	if(friction.r > 0.f)
	{
		friction.r = 1.f;
	}
	return friction.r;
}

