include $(TOPDIR)/rules.mk

PKG_NAME:=tuya-daemon
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/tuya-daemon
	CATEGORY:=Base system
	TITLE:=Router connected to Tuya IoT
	DEPENDS:=+libTuya +libubox +libubus +libblobmsg-json +luci-app-tuya-iot
endef

define Package/tuya-daemon/description
	This is daemon that makes router connected to Tuya IoT platform
endef

define Package/tuya-daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuya_daemon $(1)/usr/bin
	$(INSTALL_BIN) ./files/tuya_daemon.init $(1)/etc/init.d/tuya_daemon
	$(INSTALL_CONF) ./files/tuya_daemon.config $(1)/etc/config/tuya_daemon
endef

$(eval $(call BuildPackage,tuya-daemon))