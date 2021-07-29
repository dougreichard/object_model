import glm
import sbs

#help(glm)
#help(artemis)

player = 0
#sim = sbs.get_simulation()
def  HandleScriptStart(sim):
    global player
    v = glm.vec3(0.0, 0.0, 0.0)
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

