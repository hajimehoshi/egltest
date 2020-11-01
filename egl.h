#include <EGL/egl.h>

bool initializeEGL(EGLNativeDisplayType nativedisplay, EGLNativeWindowType nativewindow, EGLDisplay *display, EGLContext *context, EGLSurface *surface);
void destroyEGL(EGLDisplay *display, EGLContext *context, EGLSurface *surface);
