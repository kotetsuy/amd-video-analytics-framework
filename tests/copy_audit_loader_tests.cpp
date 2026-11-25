#include <dlfcn.h>
#include <va/va.h>
#include <iostream>

int main() {
    // Match applications that load libva only through a local GStreamer plugin.
    auto *library = dlopen("libva.so.2", RTLD_NOW | RTLD_LOCAL);
    auto create = reinterpret_cast<decltype(&vaCreateSurfaces)>(
        dlsym(RTLD_DEFAULT, "vaCreateSurfaces"));
    if (!library || !create) {
        std::cerr << "VA library or audit hook unavailable\n";
        return 1;
    }
    VASurfaceID surface{};
    const auto status = create(nullptr, VA_RT_FORMAT_YUV420, 2, 2, &surface, 1, nullptr, 0);
    dlclose(library);
    if (status != VA_STATUS_ERROR_INVALID_DISPLAY) {
        std::cerr << "Audit hook did not forward to libva: " << status << '\n';
        return 1;
    }
    return 0;
}
