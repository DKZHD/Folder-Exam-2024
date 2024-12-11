
for i = 0,9,1 do
    for j = 0, 9, 1 do
        local id = EntityManager:AddEntity()
        GenerateMesh(id,"Sphere",vec3((j-5)*0.25,1,i-5),vec3(1))
        addUiFloats(string.format("Mesh %d Position",id),id) 
    end
end