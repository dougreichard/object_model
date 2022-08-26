import glm
import sbs
import example

#help(glm)
#help(artemis)

player = 0
#sim = sbs.get_simulation()
def  HandleScriptStart(sim):
    global player
    v = glm.vec3(1.0, 2.0, 3.0)
    print(f"v = {v.x}, {v.y}, {v.z}")
    # copy constructor
    v1 = glm.vec3(v)
    v1.y = 300
    v2 = glm.vec3()
    v2.x = 7
    v2.y = v.x
    v2.z = 9
    # Same object
    vref = v
    vref.z = 666
    
    print(f"v = {v.x}, {v.y}, {v.z} same as vref")
    print(f"v1 = {v1.x}, {v1.y}, {v1.z}")
    print(f"v2 = {v2.x}, {v2.y}, {v2.z}")
    print(f"vref = {vref.x}, {vref.y}, {vref.z} same as v")

    print("Script start second")
    # player = sim.add_player("tsn", "artemis")
# artemis = sim.get_space_object(player)
# artemis.move(glm.vec3(2,3,4))
# print("Artemis is at X: %d Y: %d Z: %d" % (artemis.pos.x, artemis.pos.y, artemis.pos.z))



def HandleScriptTick(sim):
    global player

    print("Script tick second")
    # art = sim.get_space_object(player)
    # art.move(glm.vec3(20,30,40))
    # print("Artemis is at X: %d Y: %d Z: %d" % (art.pos.x, art.pos.y, art.pos.z))

def HandleEvents(sim, ev):
    #for e in sim.events:
    #    print(e)
    pass