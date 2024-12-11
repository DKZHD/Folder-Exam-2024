Player = {}
Player.__index = Player

function Player:new()
    local player = setmetatable({},Player)

    player.id = EntityManager:AddEntity()
    GenerateMesh(player.id,"Sphere", vec3(5,1,0),vec3(1))
    addUiFloats(string.format("Mesh %d Position", player.id),player.id)
    return player
end

Player:new()

