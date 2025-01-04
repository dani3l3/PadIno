#include "PluggableUSB.h"

class USBRename : public PluggableUSBModule {
  public:
    USBRename(
      const char *product_name = NULL,
      const char *manufacturer_name = NULL,
      const char *serial_num = NULL);
    int getInterface(uint8_t* interfaceCount);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);

  private:
    const char *manufacturer_name;
    const char *product_name;
    const char *serial_num;
    uint8_t epType[0];
};
