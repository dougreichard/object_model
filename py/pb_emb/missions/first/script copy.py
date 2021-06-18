import glm
import artemis


help(glm)
help(artemis)

player = 0
sim = artemis.get_simulation()

def  HandleScriptStart():
    global player
    global sim

    print("Script start ")
    player = sim.add_player("tsn", "artemis")
    ship = sim.get_space_object(player)
    print (player)
    print(ship.pos.x)
    ship.pos.x = 10
    p = ship.pos
    print(p.x)
    p.x = 100
    print(p.x)
    print(ship.pos.x)
    ship.move(glm.vec3(2,3,4))
    print(ship.pos.x)



def HandleScriptTick():
    global player
    global sim
    
    print("Script tick ")
    art = sim.get_space_object(player)
    art.move(glm.vec3(20,30,40))
    print("Artemis is at X: %d Y: %d Z: %d" % (art.pos.x, art.pos.y, art.pos.z))

    