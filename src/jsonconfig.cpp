//
// Created by Lee Bussy on 1/4/21
//

#include "jsonconfig.h"

Config config;
const char *filename = JSON_CONFIG_FILE;
extern const size_t capacitySerial = 3072;
extern const size_t capacityDeserial = 4096;

bool deleteConfigFile()
{
    if (!SPIFFS.begin())
    {
        return false;
    }
    return SPIFFS.remove(filename);
}

bool loadConfig()
{
    // Manage loading the configuration
    if (!loadFile())
    {
        return false;
    }
    else
    {
        saveFile();
        return true;
    }
}

bool loadFile()
{
    if (!SPIFFS.begin())
    {
        return false;
    }
    // Loads the configuration from a file on SPIFFS
    File file = SPIFFS.open(filename, "r");
    if (!SPIFFS.exists(filename) || !file)
    {
        // File does not exist or unable to read file
    }
    else
    {
        // Existing configuration present
    }

    if (!deserializeConfig(file))
    {
        file.close();
        return false;
    }
    else
    {
        file.close();
        return true;
    }
}

bool saveConfig()
{
    return saveFile();
}

bool saveFile()
{
    // Saves the configuration to a file on SPIFFS
    File file = SPIFFS.open(filename, "w");
    if (!file)
    {
        file.close();
        return false;
    }

    // Serialize JSON to file
    if (!serializeConfig(file))
    {
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool deserializeConfig(Stream &src)
{
    // Deserialize configuration
    DynamicJsonDocument doc(capacityDeserial);

    // Parse the JSON object in the file
    DeserializationError err = deserializeJson(doc, src);

    if (err)
    {
        config.load(doc.as<JsonObject>());
        return true;
    }
    else
    {
        config.load(doc.as<JsonObject>());
        return true;
    }
}

bool serializeConfig(Print &dst)
{
    // Serialize configuration
    DynamicJsonDocument doc(capacitySerial);

    // Create an object at the root
    JsonObject root = doc.to<JsonObject>();

    // Fill the object
    config.save(root);

    // Serialize JSON to file
    return serializeJson(doc, dst) > 0;
}

bool printFile()
{
    // Prints the content of a file to the Serial
    File file = SPIFFS.open(filename, "r");
    if (!file)
        return false;

    while (file.available())
        printChar(true, (const char *)file.read());

    printCR(true);
    file.close();
    return true;
}

bool printConfig()
{
    // Serialize configuration
    DynamicJsonDocument doc(capacitySerial);

    // Create an object at the root
    JsonObject root = doc.to<JsonObject>();

    // Fill the object
    config.save(root);

    bool retval = true;
    // Serialize JSON to file
    retval = serializeJson(doc, Serial) > 0;
    printCR(true);
    return retval;
}

bool mergeJsonString(String newJson)
{
    // Serialize configuration
    DynamicJsonDocument doc(capacityDeserial);

    // Parse directly from file
    DeserializationError err = deserializeJson(doc, newJson);
    if (err)
    {
        printChar(true, err.c_str());
        printCR(true);
    }

    return mergeJsonObject(doc);
}

bool mergeJsonObject(JsonVariantConst src)
{
    // Serialize configuration
    DynamicJsonDocument doc(capacityDeserial);

    // Create an object at the root
    JsonObject root = doc.to<JsonObject>();

    // Fill the object
    config.save(root);

    // Merge in the configuration
    if (merge(root, src))
    {
        // Move new object to config
        config.load(root);
        saveFile();
        return true;
    }

    return false;
}

bool merge(JsonVariant dst, JsonVariantConst src)
{
    if (src.is<JsonObject>())
    {
        for (auto kvp : src.as<JsonObject>())
        {
            merge(dst.getOrAddMember(kvp.key()), kvp.value());
        }
    }
    else
    {
        dst.set(src);
    }
    return true;
}

void Config::save(JsonObject obj) const
{
    obj["mdnsID"] = mdnsID;
    obj["invertTFT"] = invertTFT;
    obj["update_spiffs"] = update_spiffs;
    obj["TZoffset"] = TZoffset;
    obj["tempUnit"] = tempUnit;
    obj["smoothFactor"] = smoothFactor;
    obj["applyCalibration"] = applyCalibration;
    obj["tempCorrect"] = tempCorrect;

    obj["cal_red_degree"] = cal_red_degree;
    obj["cal_red_x0"] = cal_red_x0;
    obj["cal_red_x1"] = cal_red_x1;
    obj["cal_red_x2"] = cal_red_x2;
    obj["cal_red_x3"] = cal_red_x3;
    obj["cal_black_degree"] = cal_black_degree;
    obj["cal_black_x0"] = cal_black_x0;
    obj["cal_black_x1"] = cal_black_x1;
    obj["cal_black_x2"] = cal_black_x2;
    obj["cal_black_x3"] = cal_black_x3;
    obj["cal_purple_degree"] = cal_purple_degree;
    obj["cal_purple_x0"] = cal_purple_x0;
    obj["cal_purple_x1"] = cal_purple_x1;
    obj["cal_purple_x2"] = cal_purple_x2;
    obj["cal_purple_x3"] = cal_purple_x3;
    obj["cal_orange_degree"] = cal_orange_degree;
    obj["cal_orange_x0"] = cal_orange_x0;
    obj["cal_orange_x1"] = cal_orange_x1;
    obj["cal_orange_x2"] = cal_orange_x2;
    obj["cal_orange_x3"] = cal_orange_x3;
    obj["cal_blue_degree"] = cal_blue_degree;
    obj["cal_blue_x0"] = cal_blue_x0;
    obj["cal_blue_x1"] = cal_blue_x1;
    obj["cal_blue_x2"] = cal_blue_x2;
    obj["cal_blue_x3"] = cal_blue_x3;
    obj["cal_yellow_degree"] = cal_yellow_degree;
    obj["cal_yellow_x0"] = cal_yellow_x0;
    obj["cal_yellow_x1"] = cal_yellow_x1;
    obj["cal_yellow_x2"] = cal_yellow_x2;
    obj["cal_yellow_x3"] = cal_yellow_x3;
    obj["cal_pink_degree"] = cal_pink_degree;
    obj["cal_pink_x0"] = cal_pink_x0;
    obj["cal_pink_x1"] = cal_pink_x1;
    obj["cal_pink_x2"] = cal_pink_x2;
    obj["cal_pink_x3"] = cal_pink_x3;

    obj["sheetName_red"] = sheetName_red;
    obj["sheetName_green"] = sheetName_green;
    obj["sheetName_black"] = sheetName_black;
    obj["sheetName_purple"] = sheetName_purple;
    obj["sheetName_orange"] = sheetName_orange;
    obj["sheetName_blue"] = sheetName_blue;
    obj["sheetName_yellow"] = sheetName_yellow;
    obj["sheetName_pink"] = sheetName_pink;

    obj["localTargetURL"] = localTargetURL;
    obj["localTargetPushEvery"] = localTargetPushEvery;
    obj["brewstatusURL"] = brewstatusURL;
    obj["brewstatusPushEvery"] = brewstatusPushEvery;
    obj["scriptsURL"] = scriptsURL;
    obj["scriptsEmail"] = scriptsEmail;
    obj["brewersFriendKey"] = brewersFriendKey;
    obj["brewfatherKey"] = brewfatherKey;
    obj["mqttBrokerIP"] = mqttBrokerIP;
    obj["mqttBrokerPort"] = mqttBrokerPort;
    obj["mqttUsername"] = mqttUsername;
    obj["mqttPassword"] = mqttPassword;
    obj["mqttTopic"] = mqttTopic;
    obj["mqttPushEvery"] = mqttPushEvery;
}

void Config::load(JsonObjectConst obj)
{
    // Load all config objects
    //
    if (obj["mdnsID"].isNull())
    {
        strlcpy(mdnsID, "tiltbridge", sizeof(mdnsID));
    }
    else
    {
        const char *md = obj["mdnsID"];
        strlcpy(mdnsID, md, sizeof(mdnsID));
    }

	if (obj["invertTFT"].isNull())
	{
		invertTFT = false;
	}
	else
	{
		invertTFT = obj["invertTFT"];
	}

	if (obj["update_spiffs"].isNull())
	{
		update_spiffs = false;
	}
	else
	{
		update_spiffs = obj["update_spiffs"];
	}

    if (obj["TZoffset"].isNull())
    {
        TZoffset = -5;
    }
    else
    {
        int to = obj["TZoffset"];
        TZoffset = to;
    }

    if (obj["tempUnit"].isNull())
    {
        strlcpy(tempUnit, "F", sizeof(tempUnit));
    }
    else
    {
        const char *tu = obj["tempUnit"];
        strlcpy(tempUnit, tu, sizeof(tempUnit));
    }

    if (obj["smoothFactor"].isNull())
    {
        smoothFactor = 60;
    }
    else
    {
        int sf = obj["smoothFactor"];
        smoothFactor = sf;
    }

	if (obj["applyCalibration"].isNull())
	{
		applyCalibration = false;
	}
	else
	{
		applyCalibration = obj["applyCalibration"];
	}

	if (obj["tempCorrect"].isNull())
	{
		tempCorrect = false;
	}
	else
	{
		tempCorrect = obj["tempCorrect"];
	}

    if (obj["cal_red_degree"].isNull())
    {
        cal_red_degree = 1;
    }
    else
    {
        int rd = obj["cal_red_degree"];
        cal_red_degree = rd;
    }

    if (obj["cal_red_x0"].isNull())
    {
        cal_red_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_red_x0"];
        cal_red_x0 = rc;
    }

    if (obj["cal_red_x1"].isNull())
    {
        cal_red_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_red_x1"];
        cal_red_x1 = rc;
    }

    if (obj["cal_red_x2"].isNull())
    {
        cal_red_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_red_x2"];
        cal_red_x2 = rc;
    }

    if (obj["cal_red_x3"].isNull())
    {
        cal_red_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_red_x3"];
        cal_red_x3 = rc;
    }

    if (obj["cal_green_degree"].isNull())
    {
        cal_green_degree = 1;
    }
    else
    {
        int rd = obj["cal_green_degree"];
        cal_green_degree = rd;
    }

    if (obj["cal_green_x0"].isNull())
    {
        cal_green_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_green_x0"];
        cal_green_x0 = rc;
    }

    if (obj["cal_green_x1"].isNull())
    {
        cal_green_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_green_x1"];
        cal_green_x1 = rc;
    }

    if (obj["cal_green_x2"].isNull())
    {
        cal_green_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_green_x2"];
        cal_green_x2 = rc;
    }

    if (obj["cal_green_x3"].isNull())
    {
        cal_green_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_green_x3"];
        cal_green_x3 = rc;
    }

    if (obj["cal_black_degree"].isNull())
    {
        cal_black_degree = 1;
    }
    else
    {
        int rd = obj["cal_black_degree"];
        cal_black_degree = rd;
    }

    if (obj["cal_black_x0"].isNull())
    {
        cal_black_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_black_x0"];
        cal_black_x0 = rc;
    }

    if (obj["cal_black_x1"].isNull())
    {
        cal_black_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_black_x1"];
        cal_black_x1 = rc;
    }

    if (obj["cal_black_x2"].isNull())
    {
        cal_black_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_black_x2"];
        cal_black_x2 = rc;
    }

    if (obj["cal_black_x3"].isNull())
    {
        cal_black_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_black_x3"];
        cal_black_x3 = rc;
    }

    if (obj["cal_purple_degree"].isNull())
    {
        cal_purple_degree = 1;
    }
    else
    {
        int rd = obj["cal_purple_degree"];
        cal_purple_degree = rd;
    }

    if (obj["cal_purple_x0"].isNull())
    {
        cal_purple_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_purple_x0"];
        cal_purple_x0 = rc;
    }

    if (obj["cal_purple_x1"].isNull())
    {
        cal_purple_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_purple_x1"];
        cal_purple_x1 = rc;
    }

    if (obj["cal_purple_x2"].isNull())
    {
        cal_purple_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_purple_x2"];
        cal_purple_x2 = rc;
    }

    if (obj["cal_purple_x3"].isNull())
    {
        cal_purple_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_purple_x3"];
        cal_purple_x3 = rc;
    }

    if (obj["cal_orange_degree"].isNull())
    {
        cal_orange_degree = 1;
    }
    else
    {
        int rd = obj["cal_orange_degree"];
        cal_orange_degree = rd;
    }

    if (obj["cal_orange_x0"].isNull())
    {
        cal_orange_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_orange_x0"];
        cal_orange_x0 = rc;
    }

    if (obj["cal_orange_x1"].isNull())
    {
        cal_orange_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_orange_x1"];
        cal_orange_x1 = rc;
    }

    if (obj["cal_orange_x2"].isNull())
    {
        cal_orange_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_orange_x2"];
        cal_orange_x2 = rc;
    }

    if (obj["cal_orange_x3"].isNull())
    {
        cal_orange_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_orange_x3"];
        cal_orange_x3 = rc;
    }

    if (obj["cal_blue_degree"].isNull())
    {
        cal_blue_degree = 1;
    }
    else
    {
        int rd = obj["cal_blue_degree"];
        cal_blue_degree = rd;
    }

    if (obj["cal_blue_x0"].isNull())
    {
        cal_blue_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_blue_x0"];
        cal_blue_x0 = rc;
    }

    if (obj["cal_blue_x1"].isNull())
    {
        cal_blue_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_blue_x1"];
        cal_blue_x1 = rc;
    }

    if (obj["cal_blue_x2"].isNull())
    {
        cal_blue_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_blue_x2"];
        cal_blue_x2 = rc;
    }

    if (obj["cal_blue_x3"].isNull())
    {
        cal_blue_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_blue_x3"];
        cal_blue_x3 = rc;
    }

    if (obj["cal_yellow_degree"].isNull())
    {
        cal_yellow_degree = 1;
    }
    else
    {
        int rd = obj["cal_yellow_degree"];
        cal_yellow_degree = rd;
    }

    if (obj["cal_yellow_x0"].isNull())
    {
        cal_yellow_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_yellow_x0"];
        cal_yellow_x0 = rc;
    }

    if (obj["cal_yellow_x1"].isNull())
    {
        cal_yellow_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_yellow_x1"];
        cal_yellow_x1 = rc;
    }

    if (obj["cal_yellow_x2"].isNull())
    {
        cal_yellow_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_yellow_x2"];
        cal_yellow_x2 = rc;
    }

    if (obj["cal_yellow_x3"].isNull())
    {
        cal_yellow_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_yellow_x3"];
        cal_yellow_x3 = rc;
    }

    if (obj["cal_pink_degree"].isNull())
    {
        cal_pink_degree = 1;
    }
    else
    {
        int rd = obj["cal_pink_degree"];
        cal_pink_degree = rd;
    }

    if (obj["cal_pink_x0"].isNull())
    {
        cal_pink_x0 = 0.0;
    }
    else
    {
        float rc = obj["cal_pink_x0"];
        cal_pink_x0 = rc;
    }

    if (obj["cal_pink_x1"].isNull())
    {
        cal_pink_x1 = 1.0;
    }
    else
    {
        float rc = obj["cal_pink_x1"];
        cal_pink_x1 = rc;
    }

    if (obj["cal_pink_x2"].isNull())
    {
        cal_pink_x2 = 0.0;
    }
    else
    {
        float rc = obj["cal_pink_x2"];
        cal_pink_x2 = rc;
    }

    if (obj["cal_pink_x3"].isNull())
    {
        cal_pink_x3 = 0.0;
    }
    else
    {
        float rc = obj["cal_pink_x3"];
        cal_pink_x3 = rc;
    }

    if (obj["sheetName_red"].isNull())
    {
        strlcpy(sheetName_red, "", sizeof(sheetName_red));
    }
    else
    {
        const char *sn = obj["sheetName_red"];
        strlcpy(sheetName_red, sn, sizeof(sheetName_red));
    }

    if (obj["sheetName_green"].isNull())
    {
        strlcpy(sheetName_green, "", sizeof(sheetName_green));
    }
    else
    {
        const char *sn = obj["sheetName_green"];
        strlcpy(sheetName_green, sn, sizeof(sheetName_green));
    }

    if (obj["sheetName_black"].isNull())
    {
        strlcpy(sheetName_black, "", sizeof(sheetName_black));
    }
    else
    {
        const char *sn = obj["sheetName_black"];
        strlcpy(sheetName_black, sn, sizeof(sheetName_black));
    }

    if (obj["sheetName_purple"].isNull())
    {
        strlcpy(sheetName_purple, "", sizeof(sheetName_purple));
    }
    else
    {
        const char *sn = obj["sheetName_purple"];
        strlcpy(sheetName_purple, sn, sizeof(sheetName_purple));
    }

    if (obj["sheetName_orange"].isNull())
    {
        strlcpy(sheetName_orange, "", sizeof(sheetName_orange));
    }
    else
    {
        const char *sn = obj["sheetName_orange"];
        strlcpy(sheetName_orange, sn, sizeof(sheetName_orange));
    }

    if (obj["sheetName_blue"].isNull())
    {
        strlcpy(sheetName_blue, "", sizeof(sheetName_blue));
    }
    else
    {
        const char *sn = obj["sheetName_blue"];
        strlcpy(sheetName_blue, sn, sizeof(sheetName_blue));
    }

    if (obj["sheetName_yellow"].isNull())
    {
        strlcpy(sheetName_yellow, "", sizeof(sheetName_yellow));
    }
    else
    {
        const char *sn = obj["sheetName_yellow"];
        strlcpy(sheetName_yellow, sn, sizeof(sheetName_yellow));
    }

    if (obj["sheetName_pink"].isNull())
    {
        strlcpy(sheetName_pink, "", sizeof(sheetName_pink));
    }
    else
    {
        const char *sn = obj["sheetName_pink"];
        strlcpy(sheetName_pink, sn, sizeof(sheetName_pink));
    }

    if (obj["localTargetURL"].isNull())
    {
        strlcpy(localTargetURL, "", sizeof(localTargetURL));
    }
    else
    {
        const char *tu = obj["localTargetURL"];
        strlcpy(localTargetURL, tu, sizeof(localTargetURL));
    }

    if (obj["localTargetPushEvery"].isNull())
    {
        localTargetPushEvery = 30;
    }
    else
    {
        int pe = obj["localTargetPushEvery"];
        localTargetPushEvery = pe;
    }

    if (obj["brewstatusURL"].isNull())
    {
        strlcpy(brewstatusURL, "", sizeof(brewstatusURL));
    }
    else
    {
        const char *bu = obj["brewstatusURL"];
        strlcpy(brewstatusURL, bu, sizeof(brewstatusURL));
    }

    if (obj["brewstatusPushEvery"].isNull())
    {
        brewstatusPushEvery = 30;
    }
    else
    {
        int pe = obj["brewstatusPushEvery"];
        brewstatusPushEvery = pe;
    }

    if (obj["scriptsURL"].isNull())
    {
        strlcpy(scriptsURL, "", sizeof(scriptsURL));
    }
    else
    {
        const char *su = obj["scriptsURL"];
        strlcpy(scriptsURL, su, sizeof(scriptsURL));
    }

    if (obj["scriptsEmail"].isNull())
    {
        strlcpy(scriptsEmail, "", sizeof(scriptsEmail));
    }
    else
    {
        const char *se = obj["scriptsEmail"];
        strlcpy(scriptsEmail, se, sizeof(scriptsEmail));
    }

    if (obj["brewersFriendKey"].isNull())
    {
        strlcpy(brewersFriendKey, "", sizeof(brewersFriendKey));
    }
    else
    {
        const char *bf = obj["brewersFriendKey"];
        strlcpy(brewersFriendKey, bf, sizeof(brewersFriendKey));
    }

    if (obj["brewfatherKey"].isNull())
    {
        strlcpy(brewfatherKey, "", sizeof(brewfatherKey));
    }
    else
    {
        const char *bk = obj["brewfatherKey"];
        strlcpy(brewfatherKey, bk, sizeof(brewfatherKey));
    }

    if (obj["mqttBrokerIP"].isNull())
    {
        strlcpy(mqttBrokerIP, "", sizeof(mqttBrokerIP));
    }
    else
    {
        const char *mi = obj["mqttBrokerIP"];
        strlcpy(mqttBrokerIP, mi, sizeof(mqttBrokerIP));
    }

    if (obj["mqttBrokerPort"].isNull())
    {
        mqttBrokerPort = 1883;
    }
    else
    {
        int mp = obj["mqttBrokerPort"];
        mqttBrokerPort = mp;
    }

    if (obj["mqttUsername"].isNull())
    {
        strlcpy(mqttUsername, "", sizeof(mqttUsername));
    }
    else
    {
        const char *mu = obj["mqttUsername"];
        strlcpy(mqttUsername, mu, sizeof(mqttUsername));
    }

    if (obj["mqttPassword"].isNull())
    {
        strlcpy(mqttPassword, "", sizeof(mqttPassword));
    }
    else
    {
        const char *mp = obj["mqttPassword"];
        strlcpy(mqttPassword, mp, sizeof(mqttPassword));
    }

    if (obj["mqttTopic"].isNull())
    {
        strlcpy(mqttTopic, "tiltbridge", sizeof(mqttTopic));
    }
    else
    {
        const char *mt = obj["mqttTopic"];
        strlcpy(mqttTopic, mt, sizeof(mqttTopic));
    }

    if (obj["mqttPushEvery"].isNull())
    {
        mqttPushEvery = 30;
    }
    else
    {
        int me = obj["mqttPushEvery"];
        mqttPushEvery = me;
    }
}
