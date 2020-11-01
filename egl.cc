#include <stdlib.h>
#include <stdio.h>

#include "egl.h"

bool initializeEGL(EGLNativeDisplayType nativedisplay, EGLNativeWindowType nativewindow, EGLDisplay *display, EGLContext *context, EGLSurface *surface)
{
  *display = eglGetDisplay(nativedisplay);
  if (*display == EGL_NO_DISPLAY) {
    fprintf(stderr, "eglGetDisplay failed: %d\n", eglGetError());
    return false;
  }

  if (!eglInitialize(*display, nullptr, nullptr)) {
    fprintf(stderr, "eglInitialize failed: %d\n", eglGetError());
    return false;
  }

  EGLint attr[] = {
    EGL_BUFFER_SIZE, 32,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE};
  EGLConfig config = nullptr;
  EGLint numConfigs = 0;
  if (!eglChooseConfig(*display, attr, &config, 1, &numConfigs)) {
    fprintf(stderr, "eglChooseConfig failed: %d\n", eglGetError());
    return false;
  }
  if (numConfigs != 1) {
    fprintf(stderr, "eglChooseConfig failed (numConfigs must be 1 but %d): %d\n", numConfigs, eglGetError());
    return false;
  }

  *surface = eglCreateWindowSurface(*display, config, nativewindow, nullptr);
  if (*surface == EGL_NO_SURFACE) {
    fprintf(stderr, "eglCreateWindowSurface failed: %d\n", eglGetError());
    return false;
  }

  EGLint ctxattr[] = {EGL_CONTEXT_MAJOR_VERSION, 2, EGL_CONTEXT_MINOR_VERSION, 1, EGL_NONE};
  *context = eglCreateContext(*display, config, EGL_NO_CONTEXT, ctxattr);
  if (*context == EGL_NO_CONTEXT) {
    fprintf(stderr, "eglCreateContext failed: %d\n", eglGetError());
    return false;
  }
  eglMakeCurrent(*display, *surface, *surface, *context);

  return true;
}

void destroyEGL(EGLDisplay *display, EGLContext *context, EGLSurface *surface)
{
  eglDestroyContext(display, context);
  eglDestroySurface(display, surface);
  eglTerminate(display);
}
