import glm
import sbs
import sbsevent
import example

#help(glm)
#help(artemis)

player = 0
#sim = sbs.get_simulation()

def  HandleScriptStart(sim):
    global player
    # print("Script start ")
    # player = sim.add_player("tsn", "artemis")
    # artemis = sim.get_space_object(player)
    # artemis.move(glm.vec3(2,3,4))
    # print("Artemis is at X: %d Y: %d Z: %d" % (artemis.pos.x, artemis.pos.y, artemis.pos.z))



def HandleScriptTick(sim):
    global player

    print(len(sim.get_events()))
    for e in sim.get_events4():
       print(e)
       print(e.type)
       if e.type == sbsevent.EventType.PRESENT_GUI_MESSAGE:
          print(e.message_tag)
    
    # print("Script tick ")
    # art = sim.get_space_object(player)
    # art.move(glm.vec3(20,30,40))
    # print("Artemis is at X: %d Y: %d Z: %d" % (art.pos.x, art.pos.y, art.pos.z))


def HandleEvents(sim):
    print(len(sim.get_events()))
    for e in sim.get_events():
       print(e)
       pass