#import glm
#import sbs
from threading import Thread, Event

thread_event = Event()

import time


from sdl2 import *
import ctypes
import OpenGL.GL as gl

import imgui
from imgui.integrations.sdl2 import SDL2Renderer


class MyGUI:
    def __init__(self):
        # Create an object of the above class
        pass

        # Start a gui on a thread
    def gui_thread():
        gui= MyGUI()
        gui.main()
    

    def main(self):
        window, gl_context = self.impl_pysdl2_init()
        imgui.create_context()
        impl = SDL2Renderer(window)

        self.running = True
        event = SDL_Event()
        while self.running:
            if thread_event.is_set():
                self.running = False
                break

            while SDL_PollEvent(ctypes.byref(event)) != 0:
                if event.type == SDL_QUIT:
                    self.running = False
                    break
                impl.process_event(event)
            impl.process_inputs()

            imgui.new_frame()

            if imgui.begin_main_menu_bar():
                if imgui.begin_menu("File", True):

                    clicked_quit, selected_quit = imgui.menu_item(
                        "Quit", 'Cmd+Q', False, True
                    )

                    if clicked_quit:
                        exit(1)

                    imgui.end_menu()
                imgui.end_main_menu_bar()

            #show_test_window()
            #imgui.show_test_window()

            imgui.begin("Custom window", True)
            imgui.text("Bar")
            imgui.text_colored("Eggs", 0.2, 1., 0.)
            imgui.end()

            gl.glClearColor(1., 1., 1., 1)
            gl.glClear(gl.GL_COLOR_BUFFER_BIT)

            imgui.render()
            impl.render(imgui.get_draw_data())
            SDL_GL_SwapWindow(window)

        impl.shutdown()
        SDL_GL_DeleteContext(gl_context)
        SDL_DestroyWindow(window)
        SDL_Quit()


    def impl_pysdl2_init(self):
        width, height = 1280, 720
        window_name = "minimal ImGui/SDL2 example"

        if SDL_Init(SDL_INIT_EVERYTHING) < 0:
            print("Error: SDL could not initialize! SDL Error: " + SDL_GetError().decode("utf-8"))
            exit(1)

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8)
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1)
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1)
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)

        SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, b"1")
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, b"1")

        window = SDL_CreateWindow(window_name.encode('utf-8'),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height,
                                SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE)

        if window is None:
            print("Error: Window could not be created! SDL Error: " + SDL_GetError().decode("utf-8"))
            exit(1)

        gl_context = SDL_GL_CreateContext(window)
        if gl_context is None:
            print("Error: Cannot create OpenGL Context! SDL Error: " + SDL_GetError().decode("utf-8"))
            exit(1)

        SDL_GL_MakeCurrent(window, gl_context)
        if SDL_GL_SetSwapInterval(1) < 0:
            print("Warning: Unable to set VSync! SDL Error: " + SDL_GetError().decode("utf-8"))
            exit(1)

        return window, gl_context





def  HandleScriptStart(sim):
    global player
    print('starting first')
    x = Thread(target=MyGUI.gui_thread)
    x.daemon = True
    x.start()
    # data = {"text": "Hello world github/linguist№1 **cool**, and #1!"}
    # response = urlopen("https://api.github.com/markdown", json.dumps(data).encode())
    # print(response.read().decode())

    
    print('started gui')



def HandleScriptTick(sim):
    # global player
    # global myserver
    # myserver.server.handle_request()
    
    print("Script tick ")
    # art = sim.get_space_object(player)
    # art.move(glm.vec3(20,30,40))
    # print("Artemis is at X: %d Y: %d Z: %d" % (art.pos.x, art.pos.y, art.pos.z))


if __name__ == "__main__":
    HandleScriptStart("none")

    for _ in range(0,10):
        time.sleep(1)
    print("Exiting")
