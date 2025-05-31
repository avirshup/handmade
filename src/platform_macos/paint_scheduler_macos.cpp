// #include "paint_scheduler_macos.h"
#include "../common.h"

/*** ChatGPT output, not verified:

#include <CoreFoundation/CoreFoundation.h>
#include <CoreVideo/CoreVideo.h>

// Your callback function
CVReturn DisplayLinkCallback(
    CVDisplayLinkRef displayLink,
    const CVTimeStamp* now,
    const CVTimeStamp* outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags* flagsOut,
    void* displayLinkContext) {
  std::cout << "Display refreshed at time: " << outputTime->videoTime
            << std::endl;

  // Cast context if needed to access your app state
  return kCVReturnSuccess;
}

int main() {
  CVDisplayLinkRef displayLink;

  // Create the display link (for main display)
  if (CVDisplayLinkCreateWithActiveCGDisplays(&displayLink) !=
      kCVReturnSuccess) {
    std::cerr << "Failed to create display link" << std::endl;
    return -1;
  }

  // Set the callback
  CVDisplayLinkSetOutputCallback(displayLink, DisplayLinkCallback, nullptr);

  // Start the display link
  if (CVDisplayLinkStart(displayLink) != kCVReturnSuccess) {
    std::cerr << "Failed to start display link" << std::endl;
    CVDisplayLinkRelease(displayLink);
    return -1;
  }

  // Run the main loop (you need to integrate this into a real app loop)
  std::cout << "Running... Press Enter to stop." << std::endl;
  std::cin.get();

  CVDisplayLinkStop(displayLink);
  CVDisplayLinkRelease(displayLink);

  return 0;
}

end of chatgpt output */