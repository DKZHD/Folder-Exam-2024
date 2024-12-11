local id = EntityManager:AddEntity()
GenerateMesh(id,"Cube",vec3(0),vec3(25,1,25))
addUiFloats("Mesh 0 Position",id)

id = EntityManager:AddEntity()
GenerateMesh(id,"Cube",vec3(0,1,12),vec3(25,1,1))
addUiFloats("Mesh 1 Position",id)

id = EntityManager:AddEntity()
GenerateMesh(id,"Cube",vec3(0,1,-12),vec3(25,1,1))
addUiFloats("Mesh 2 Position",id)

id = EntityManager:AddEntity()
GenerateMesh(id,"Cube",vec3(12,1,0),vec3(1,1,23))
addUiFloats("Mesh 3 Position",id)

id = EntityManager:AddEntity()
GenerateMesh(id,"Cube",vec3(-12,1,0),vec3(1,1,23))
addUiFloats("Mesh 4 Position",id)



