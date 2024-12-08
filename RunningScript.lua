
-- addMesh("Cube",vec3(0,8,0),vec3(25,1,25))
-- ComponentManager:addComponent(0, "PositionComponent")

-- print("Position: x: "..pos.position.x, "y: ".. pos.position.y, "z: "..pos.position.z)

-- addMesh("Cube",vec3(50,math.random(0,100),0),vec3(1,1,1))

DecreaseHealth(1, 10)
print("Health: "..ComponentManager:getComponent(1,"HealthComponent").health)

-- local id = EntityManager:AddEntity()
-- GenerateMesh(id,"Cube",vec3(0,10,5),vec3(3,3,3))
-- addUiFloats(string.format("Mesh %d Position",id),id)