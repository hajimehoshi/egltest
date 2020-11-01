#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xutil.h>
#include <GL/gl.h>

#include "egl.h"

void mainloop(Display *xdisplay, EGLDisplay display, EGLSurface surface)
{
  for (;;) {
    XPending(xdisplay);
    glClearColor(0.25f, 0.25f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(display, surface);
    usleep(1000);
  }
}

int main(int argc, char *argv[])
{
  Display *xdisplay = XOpenDisplay(nullptr);
  if (xdisplay == nullptr) {
    fprintf(stderr, "XOpenDisplay failed\n");
    return EXIT_FAILURE;
  }

  Window xwindow = XCreateSimpleWindow(xdisplay, DefaultRootWindow(xdisplay), 100, 100, 640, 480,
                                       1, BlackPixel(xdisplay, 0), WhitePixel(xdisplay, 0));
  XMapWindow(xdisplay, xwindow);

  EGLDisplay display = nullptr;
  EGLContext context = nullptr;
  EGLSurface surface = nullptr;
  if (!initializeEGL(xdisplay, xwindow, &display, &context, &surface)) {
    return EXIT_FAILURE;
  }

  mainloop(xdisplay, display, surface);

  destroyEGL(&display, &context, &surface);
  XDestroyWindow(xdisplay, xwindow);
  XCloseDisplay(xdisplay);

  return EXIT_SUCCESS;
}
