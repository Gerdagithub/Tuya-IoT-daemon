include $(TOPDIR)/rules.mk

PKG_NAME := libTuya
PKG_RELEASE := 1
PKG_VERSION := 1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/tuya/tuya-iot-core-sdk.git
PKG_SOURCE_VERSION:=0208badefb2003d30e70b5b875395e3187c850d9

CMAKE_BINARY_SUBDIR:=build

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/libTuya
	CATEGORY := Libraries
	TITLE := libTuya
endef

define Package/libTuya/description
	This is a package of tuya libraries that can be used with other programs.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib/include
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/utils/*.h $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/interface/*.h $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/examples/subdevice_basic_demo/tuya_cacert.h $(1)/usr/include

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/*.so $(1)/usr/lib

endef

define Package/libTuya/install
	$(INSTALL_DIR) $(1)/usr/lib

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/lib/*.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,libTuya))