#include "mpv_manager.h"
#include "mpv_manager_p.h"
#include <mpv/client.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

mpv_manager_impl::mpv_manager_impl():handle(mpv_create(),mpv_handle_deleter())
{
}
mpv_manager_impl::~mpv_manager_impl()
{
}

void mpv_manager_impl::show_window()
{
  mpv_initialize(handle.get());

  
  Display *disp = XOpenDisplay(":0");
  int blackColor = BlackPixel(disp, DefaultScreen(disp));
  int whiteColor = WhitePixel(disp, DefaultScreen(disp));    
  XSizeHints myhint;
  myhint.x = 0;
  myhint.y = 0;
  myhint.width = 500;
  myhint.height = 500;
  myhint.flags = PPosition | PSize;
  
  Window win = XCreateSimpleWindow(disp,XDefaultRootWindow(disp),myhint.x,myhint.y,myhint.width,myhint.height,0,whiteColor,blackColor);
  
  //char* argv[] = {""};
  //XSetStandardProperties(disp,win,"","",None,argv,0,&myhint);
  
  XSelectInput(disp, win, ExposureMask);
  XMapWindow(disp, win);
  Atom WM_DELETE_WINDOW = XInternAtom(disp, "WM_DELETE_WINDOW", False); 
  XSetWMProtocols(disp, win, &WM_DELETE_WINDOW, 1);  
  Atom wm_state = XInternAtom(disp, "_NET_WM_STATE", False);
  Atom fullscreen = XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", False);
  XEvent xev = {0};   
  XMoveResizeWindow(disp,win,0,0,myhint.width,myhint.height);
  XMapRaised(disp,win);
  XGrabPointer(disp,win,True,0,GrabModeAsync,GrabModeAsync,win,0L,CurrentTime);
  XGrabKeyboard(disp,win,False,GrabModeAsync,GrabModeAsync,CurrentTime);
  xev.type = ClientMessage;
  xev.xclient.window = win;
  xev.xclient.message_type = wm_state;
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = 1;
  xev.xclient.data.l[1] = fullscreen;
  xev.xclient.data.l[2] = 0;
  XSendEvent (disp, DefaultRootWindow(disp), False,SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  XFlush(disp);

  mpv_set_option_string(handle.get(), "input-default-bindings", "yes");
  mpv_set_option_string(handle.get(), "input-vo-keyboard", "yes");  
  mpv_set_option(handle.get(), "wid", MPV_FORMAT_INT64, &win);
  int val = 1;
  mpv_set_option(handle.get(), "osc", MPV_FORMAT_FLAG, &val);  
  
  const char* cmd[] = {"loadfile", "/home/sergiu/Downloads/simpsons.mp4", nullptr};
  mpv_command(handle.get(),cmd);
  while (1) {
    XEvent e;
    XNextEvent(disp,&e);
    while(1)
    {
    mpv_event *mp_event = mpv_wait_event(handle.get(), 0);
    if (mp_event->event_id == MPV_EVENT_NONE)
        break;
    std::cout << "event: "<< mpv_event_name(mp_event->event_id)<<std::endl;
    }
  }
  XDestroyWindow(disp,win);
  XCloseDisplay(disp);
}

mpv_manager::mpv_manager()
    : impl(std::make_unique<mpv_manager_impl>())
{

}
mpv_manager::~mpv_manager() = default;

void mpv_manager::show_window()
{
  impl->show_window();
}
