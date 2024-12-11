
-- addMesh("Cube",vec3(0,8,0),vec3(25,1,25))
-- ComponentManager:addComponent(0, "PositionComponent")

-- print("Position: x: "..pos.position.x, "y: ".. pos.position.y, "z: "..pos.position.z)

-- addMesh("Cube",vec3(50,math.random(0,100),0),vec3(1,1,1))

-- DecreaseHealth(1, 10)
-- print("Health: "..ComponentManager:getComponent(1,"HealthComponent").health)

-- local id = EntityManager:GetLastEntity()
-- local posComp = ComponentManager:getComponent(id, "PositionComponent")
-- posComp.position.x = posComp.position.x + 0.5

-- for i = 1, 100, 1 do
--     local id = EntityManager:AddEntity()
--     GenerateMesh(id,"Sphere",vec3(math.random(0,50),math.random(10,20),math.random(25,75)),vec3(1))
--     addUiFloats(string.format("Mesh %d Position",id),id)
-- end
-- ComponentManager.addComponent(1,"MovementComponent")
AddVelocity(5,vec3(1,0,0))

-- local id = EntityManager:AddEntity()
-- GenerateMesh(id,"Cube",vec3(0,10,5),vec3(3,3,3))
-- addUiFloats(string.format("Mesh %d Position",id),id)