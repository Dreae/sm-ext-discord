// vim: set ts=4 :
#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/**
 * @file smsdk_config.h
 * @brief Contains macros for configuring basic extension information.
 */

/* Basic information exposed publically */
#define SMEXT_CONF_NAME			"Discord"
#define SMEXT_CONF_DESCRIPTION	"SourceMod extension for Discord bots"
#define SMEXT_CONF_VERSION		"0.0.1"
#define SMEXT_CONF_AUTHOR		"Dreae"
#define SMEXT_CONF_URL			"https://github.com/dreae/sm-ext-discord"
#define SMEXT_CONF_LOGTAG		"Dscord"
#define SMEXT_CONF_LICENSE		"MIT"
#define SMEXT_CONF_DATESTRING	__DATE__

/**
 * @brief Exposes plugin's main interface.
 */
#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

#define SMEXT_ENABLE_PLUGINSYS
#define SMEXT_ENABLE_HANDLESYS

#endif //_INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

