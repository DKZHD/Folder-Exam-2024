Player = {}
Player.__index = Player

function Player:new()
    local player = setmetatable({},Player)

    player.id = EntityManager:AddEntity()
    GenerateMesh(player.id,"Cube", vec3(0,2,0),vec3(1))
    ComponentManager:addComponent(player.id,"HealthComponent")
    ComponentManager:addComponent(player.id,"MovementComponent")
    addUiFloats(string.format("Mesh %d Position", player.id),player.id)
    return player
end

Player:new()