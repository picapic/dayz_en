-- MySQL dump 10.13  Distrib 5.6.10, for Win32 (x86)
--
-- Host: localhost    Database: dayz
-- ------------------------------------------------------
-- Server version	5.6.10

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `characters`
--

DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `player_id` int(11) NOT NULL DEFAULT '0',
  `health` int(11) NOT NULL DEFAULT '12000',
  `hunger` int(11) NOT NULL DEFAULT '1000',
  `thirst` int(11) NOT NULL DEFAULT '1000',
  `wound` int(11) NOT NULL DEFAULT '0',
  `placex` float NOT NULL DEFAULT '-1420.64',
  `placey` float NOT NULL DEFAULT '-2897.94',
  `placez` float NOT NULL DEFAULT '48.0911',
  `angle` float NOT NULL DEFAULT '38.8824',
  `skin` int(11) NOT NULL DEFAULT '2',
  `killer_id` int(11) DEFAULT NULL,
  `c_killer` int(11) DEFAULT NULL,
  `scores` int(11) NOT NULL DEFAULT '0',
  `bplacex` float NOT NULL DEFAULT '-1420.64',
  `bplacey` float NOT NULL DEFAULT '-2897.94',
  `bplacez` float NOT NULL DEFAULT '48.0911',
  `bangle` float NOT NULL DEFAULT '38.8824',
  `cheater` int not null default 0,
  KEY `player_id` (`player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dot_type`
--

DROP TABLE IF EXISTS `dot_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dot_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dot_id` int(11) NOT NULL DEFAULT '0',
  `type_id` int(11) NOT NULL DEFAULT '0',
  UNIQUE KEY `dot_id` (`dot_id`,`type_id`),
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2531 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inventory`
--

DROP TABLE IF EXISTS `inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventory` (
  `player_id` int(11) NOT NULL DEFAULT '0',
  `inv1` int(11) NOT NULL DEFAULT '-1',
  `inv2` int(11) NOT NULL DEFAULT '-1',
  `inv3` int(11) NOT NULL DEFAULT '-1',
  `inv4` int(11) NOT NULL DEFAULT '-1',
  `inv5` int(11) NOT NULL DEFAULT '-1',
  `inv6` int(11) NOT NULL DEFAULT '-1',
  KEY `id` (`player_id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `object_dot`
--

DROP TABLE IF EXISTS `object_dot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `object_dot` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `placex` float NOT NULL DEFAULT '0',
  `placey` float NOT NULL DEFAULT '0',
  `placez` float NOT NULL DEFAULT '0',
  `object_id` int(11) DEFAULT NULL,
  `last_time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1389 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `objects`
--

DROP TABLE IF EXISTS `objects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `objects` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `thing_id` int(11) NOT NULL DEFAULT '0',
  `next_id` int(11) NOT NULL DEFAULT '0',
  `prev_id` int(11) DEFAULT NULL,
  `placex` float DEFAULT NULL,
  `placey` float DEFAULT NULL,
  `placez` float DEFAULT NULL,
  `add_rotx` float NOT NULL DEFAULT '0',
  `add_roty` float NOT NULL DEFAULT '0',
  `add_rotz` float NOT NULL DEFAULT '0',
  `pl_owner_id` int(11) DEFAULT NULL,
  `th_owner_id` int(11) DEFAULT NULL,
  `obj_id` int(11) DEFAULT NULL,
  `dot_id` int(11) DEFAULT NULL,
  `is_dropped` int(11) DEFAULT NULL,
  `unused` int(11) NOT NULL DEFAULT '0',
  `value` int(11) NOT NULL DEFAULT '0',
  `vector` float DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `id` (`id`),
  KEY `thing_id` (`thing_id`)
) ENGINE=MyISAM AUTO_INCREMENT=1890 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `players`
--

DROP TABLE IF EXISTS `players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `players` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(24) CHARACTER SET latin1 NOT NULL DEFAULT '',
  `passwd` varchar(32) CHARACTER SET latin1 NOT NULL DEFAULT '',
  `reg_ip` varchar(15) CHARACTER SET latin1 NOT NULL DEFAULT '',
  `last_ip` varchar(15) CHARACTER SET latin1 NOT NULL DEFAULT '',
  `reg_ip_int` int(11) NOT NULL DEFAULT '0',
  `last_ip_int` int(11) NOT NULL DEFAULT '0',
  `reg_date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`name`),
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=81 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `spawns`
--

DROP TABLE IF EXISTS `spawns`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spawns` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `placex` float NOT NULL DEFAULT '0',
  `placey` float NOT NULL DEFAULT '0',
  `placez` float NOT NULL DEFAULT '0',
  `angle` float NOT NULL DEFAULT '0',
  `last_time` datetime DEFAULT NULL,
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `thing_type`
--

DROP TABLE IF EXISTS `thing_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `thing_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) CHARACTER SET cp1251 NOT NULL DEFAULT '',
  `about` varchar(64) CHARACTER SET cp1251 DEFAULT NULL,
  PRIMARY KEY (`name`),
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `things`
--

DROP TABLE IF EXISTS `things`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `things` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) CHARACTER SET latin1 NOT NULL DEFAULT '',
  `is_inventory` int(11) NOT NULL DEFAULT '0',
  `is_auto` int(11) NOT NULL DEFAULT '0',
  `is_vehicle` int(11) NOT NULL DEFAULT '0',
  `rotx` int(11) NOT NULL DEFAULT '0',
  `roty` int(11) NOT NULL DEFAULT '0',
  `rotz` int(11) NOT NULL DEFAULT '0',
  `inventx` float NOT NULL DEFAULT '0',
  `inventy` float NOT NULL DEFAULT '0',
  `inventz` float NOT NULL DEFAULT '0',
  `zoom` float NOT NULL DEFAULT '1',
  `posx` float NOT NULL DEFAULT '0',
  `posy` float NOT NULL DEFAULT '0',
  `posz` float NOT NULL DEFAULT '0',
  `height` float NOT NULL DEFAULT '0',
  `invent_id` int(11) NOT NULL DEFAULT '0',
  `inworld_id` int(11) NOT NULL DEFAULT '0',
  `rotatable` int(11) NOT NULL DEFAULT '0',
  `def_value` int(11) NOT NULL DEFAULT '0',
  `type_id` int(11) NOT NULL DEFAULT '1',
  `is_consumble` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`name`),
  KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=10308 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `veh_data`
--

DROP TABLE IF EXISTS `veh_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `veh_data` (
  `object_id` int(11) NOT NULL DEFAULT '0',
  `color1` int(11) NOT NULL DEFAULT '0',
  `color2` int(11) NOT NULL DEFAULT '0',
  `panels` int(11) NOT NULL DEFAULT '0',
  `doors` int(11) NOT NULL DEFAULT '0',
  `light` int(11) NOT NULL DEFAULT '0',
  `tires` int(11) NOT NULL DEFAULT '0',
  `patrol` int(11) NOT NULL DEFAULT '0',
  `is_working` int(11) NOT NULL DEFAULT '0',
  `engine_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`object_id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `veh_invent`
--

DROP TABLE IF EXISTS `veh_invent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `veh_invent` (
  `object_id` int(11) NOT NULL AUTO_INCREMENT,
  `inv1` int(11) NOT NULL DEFAULT '-1',
  `inv2` int(11) NOT NULL DEFAULT '-1',
  `inv3` int(11) NOT NULL DEFAULT '-1',
  `inv4` int(11) NOT NULL DEFAULT '-1',
  `inv5` int(11) NOT NULL DEFAULT '-1',
  `inv6` int(11) NOT NULL DEFAULT '-1',
  `inv7` int(11) NOT NULL DEFAULT '-1',
  `inv8` int(11) NOT NULL DEFAULT '-1',
  `inv9` int(11) NOT NULL DEFAULT '-1',
  `inv10` int(11) NOT NULL DEFAULT '-1',
  `inv11` int(11) NOT NULL DEFAULT '-1',
  `inv12` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`object_id`)
) ENGINE=MyISAM AUTO_INCREMENT=721 DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vehicles`
--

DROP TABLE IF EXISTS `vehicles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vehicles` (
  `thing_id` int(11) NOT NULL DEFAULT '0',
  `cells` int(11) NOT NULL DEFAULT '4',
  `wheels` int(11) NOT NULL DEFAULT '4',
  `def_panels` int(11) NOT NULL DEFAULT '0',
  `def_doors` int(11) NOT NULL DEFAULT '0',
  `def_light` int(11) NOT NULL DEFAULT '0',
  `def_tires` int(11) NOT NULL DEFAULT '0',
  `def_patrol` int(11) NOT NULL DEFAULT '0',
  `patrol_cons` int(11) NOT NULL DEFAULT '10',
  `max_patrol` int(11) NOT NULL DEFAULT '1000',
  PRIMARY KEY (`thing_id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-10-13 20:18:45
