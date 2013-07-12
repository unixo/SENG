-- MySQL dump 10.13  Distrib 5.1.48, for apple-darwin10.3.0 (i386)
--
-- Host: localhost    Database: seng
-- ------------------------------------------------------
-- Server version	5.1.48

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
-- Current Database: `seng`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `seng` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `seng`;

--
-- Temporary table structure for view `available_products`
--

DROP TABLE IF EXISTS `available_products`;
/*!50001 DROP VIEW IF EXISTS `available_products`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `available_products` (
  `pid` int(11),
  `cid` int(11),
  `name` varchar(45),
  `descr` tinytext,
  `price` float,
  `availability` tinyint(4),
  `deleted` tinyint(1)
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Temporary table structure for view `catalogue`
--

DROP TABLE IF EXISTS `catalogue`;
/*!50001 DROP VIEW IF EXISTS `catalogue`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `catalogue` (
  `pid` int(11),
  `cid` int(11),
  `name` varchar(45),
  `descr` tinytext,
  `price` float,
  `availability` tinyint(4),
  `deleted` tinyint(1),
  `category` varchar(45)
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `categories`
--

DROP TABLE IF EXISTS `categories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `categories` (
  `cid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`cid`),
  UNIQUE KEY `cat_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `categories`
--

LOCK TABLES `categories` WRITE;
/*!40000 ALTER TABLE `categories` DISABLE KEYS */;
INSERT INTO `categories` VALUES (7,'Audio'),(6,'Computer Desktop'),(5,'Hard disk'),(3,'Keyboards'),(2,'Monitor'),(1,'Notebook'),(4,'PCCARD'),(28,'Wi-Fi Antennas');
/*!40000 ALTER TABLE `categories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `configurations`
--

DROP TABLE IF EXISTS `configurations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `configurations` (
  `oid` int(11) NOT NULL,
  `pid` int(11) NOT NULL,
  PRIMARY KEY (`oid`,`pid`),
  KEY `offers` (`oid`),
  KEY `products` (`pid`),
  CONSTRAINT `offers` FOREIGN KEY (`oid`) REFERENCES `offers` (`oid`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `products` FOREIGN KEY (`pid`) REFERENCES `products` (`pid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `configurations`
--

LOCK TABLES `configurations` WRITE;
/*!40000 ALTER TABLE `configurations` DISABLE KEYS */;
INSERT INTO `configurations` VALUES (1,3),(1,5),(1,6),(2,2),(2,5),(2,6),(2,8),(2,9),(2,10),(3,3),(3,6),(3,11);
/*!40000 ALTER TABLE `configurations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `handled_orders`
--

DROP TABLE IF EXISTS `handled_orders`;
/*!50001 DROP VIEW IF EXISTS `handled_orders`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `handled_orders` (
  `oid` int(11),
  `date` datetime,
  `uid` int(11),
  `customer` varchar(41),
  `category` varchar(45),
  `product` varchar(45),
  `qty` int(11),
  `order_total` float
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `offers`
--

DROP TABLE IF EXISTS `offers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offers` (
  `oid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`oid`),
  UNIQUE KEY `nome` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `offers`
--

LOCK TABLES `offers` WRITE;
/*!40000 ALTER TABLE `offers` DISABLE KEYS */;
INSERT INTO `offers` VALUES (3,'Desktop 4 everyone'),(2,'Full optional Offer'),(1,'Total Discount Offer');
/*!40000 ALTER TABLE `offers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order_details`
--

DROP TABLE IF EXISTS `order_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order_details` (
  `oid` int(11) NOT NULL,
  `pid` int(11) NOT NULL,
  `qty` int(11) DEFAULT '0',
  PRIMARY KEY (`oid`,`pid`),
  KEY `master_order` (`oid`),
  KEY `product` (`pid`),
  CONSTRAINT `master_order` FOREIGN KEY (`oid`) REFERENCES `orders` (`oid`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `product` FOREIGN KEY (`pid`) REFERENCES `products` (`pid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_details`
--

LOCK TABLES `order_details` WRITE;
/*!40000 ALTER TABLE `order_details` DISABLE KEYS */;
INSERT INTO `order_details` VALUES (1,3,1),(1,11,1),(2,2,1),(2,3,1),(2,5,1),(3,2,1),(3,6,1),(3,9,1),(4,6,1),(4,10,1),(4,11,1),(4,15,1),(5,6,1),(5,14,1),(18,1,2),(18,8,1),(25,1,2),(25,8,1),(25,27,1),(26,1,2),(26,15,4);
/*!40000 ALTER TABLE `order_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `orders`
--

DROP TABLE IF EXISTS `orders`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `orders` (
  `oid` int(11) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `date` datetime NOT NULL,
  `total` float DEFAULT '0',
  PRIMARY KEY (`oid`),
  KEY `users` (`uid`),
  CONSTRAINT `users` FOREIGN KEY (`uid`) REFERENCES `users` (`uid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `orders`
--

LOCK TABLES `orders` WRITE;
/*!40000 ALTER TABLE `orders` DISABLE KEYS */;
INSERT INTO `orders` VALUES (1,1,'2010-01-20 18:31:24',800),(2,2,'2009-12-20 18:31:24',1515),(3,3,'2009-12-16 18:31:24',2190),(4,4,'2009-10-20 18:31:24',2300),(5,1,'2009-07-20 18:31:24',830),(18,18,'2010-07-07 18:20:58',2393),(20,29,'2010-07-10 15:14:23',2418),(21,29,'2010-07-10 15:15:06',2418),(22,29,'2010-07-10 15:15:47',2418),(23,29,'2010-07-10 15:16:59',2418),(24,29,'2010-07-10 15:17:14',2418),(25,29,'2010-07-10 15:17:24',2418),(26,18,'2010-08-14 13:52:40',2184);
/*!40000 ALTER TABLE `orders` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `products`
--

DROP TABLE IF EXISTS `products`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `products` (
  `pid` int(11) NOT NULL AUTO_INCREMENT COMMENT '		',
  `cid` int(11) NOT NULL,
  `name` varchar(45) NOT NULL,
  `descr` tinytext,
  `price` float DEFAULT '0',
  `availability` tinyint(4) DEFAULT '0',
  `deleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`pid`),
  UNIQUE KEY `prd_name` (`name`),
  KEY `category` (`cid`),
  CONSTRAINT `category` FOREIGN KEY (`cid`) REFERENCES `categories` (`cid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `products`
--

LOCK TABLES `products` WRITE;
/*!40000 ALTER TABLE `products` DISABLE KEYS */;
INSERT INTO `products` VALUES (1,1,'Apple MacBook Pro','Monitor 15 inches + HDD 250Gb solid state',2150,10,0),(2,1,'Apple iMac 24','Desktop monitor 24 inches + HDD 640Gb',1200,2,1),(3,2,'External monitor Acer','Width 32 inches',800,1,0),(4,2,'Monitor LCD Full HD','Resolution 1080p',1230,23,0),(5,3,'Italian keyboard','102 keys',15,1,0),(6,3,'Bluetooth keyboard',NULL,45,10,0),(7,4,'Wireless card 811.2e','Compatible with Windows and Macos',123,0,0),(8,4,'TV Card','Transform your computer in a portable TV',243,10,0),(9,4,'NETGEAR WG511 Wireless-G','PC Card Network adapter',30,5,0),(10,5,'HD esterno 1.5Tb','Supporto USB1 & 2',160,10,1),(11,6,'PACKARD BELL iMax mini','HD 160GB - RAM 1GB',249,11,0),(12,6,'FUJITSU-SIEMENS Amilo Li 3740','HD 160GB - RAM 1GB',249,11,0),(13,6,'HP Pavilion Elite m9441.it','Intel Atom 230 (1.60GHz, 512KB L2, 533MHz FSB)',369,1,0),(14,7,'LOGITECH LS11','Speaker system 2.0 for Pc',19.9,3,0),(15,7,'HERCULES XPS 2.0 Lounge','Speaker system 2.0 - 10W',34.9,7,0),(19,28,'ipp','luto',3.5,0,0),(20,28,'no','ue',3.2,0,0),(22,7,'pp','qq',0,0,0),(24,7,'ppp1234','2',123,0,0),(26,28,'Extra range III','Extra range wifi extension (G)',175.2,10,0),(27,4,'USB to PCCARD converter','USB 2.0 compatible converter',25,10,0);
/*!40000 ALTER TABLE `products` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL COMMENT '	',
  `surname` varchar(20) NOT NULL,
  `login` varchar(15) NOT NULL,
  `password` varchar(45) NOT NULL,
  `address` varchar(45) NOT NULL,
  `city` varchar(15) NOT NULL,
  `admin` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `credentials` (`login`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'Ferruccio','Vitale','unixo','secret','via del Sagittario, 17F','Rome',1),(2,'Alessandro','Aldini','aaldini','secret','via Saffi, 42','Urbino',0),(3,'Erika','Pigliapoco','md','*LK*','via Saffi, 42','Urbino',0),(4,'Alessandro','Bogliolo','boglio','*LK*','via Saffi, 42','Urbino',0),(18,'Edoardo','BontÃ ','eddy','secret','via Saffi, 42','Urbino',0),(29,'Elena Silvana','Vitale','elena','secret','via Daqualche Parte 11','Rome',0);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'seng'
--
/*!50003 DROP PROCEDURE IF EXISTS `offers_by_product` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `offers_by_product`(IN p_pid INT)
BEGIN

DECLARE offer_ID    INT;
DECLARE offer_name  CHAR(45);
DECLARE offer_price INT default 0;
DECLARE done        INT default 0;

DECLARE c1 CURSOR FOR SELECT DISTINCT oid FROM configurations WHERE pid = p_pid;
DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
OPEN C1;

FETCH c1 INTO offer_ID;
myloop: WHILE NOT done DO
            
            
            SELECT name INTO offer_name FROM offers WHERE oid = offer_ID;
            SELECT SUM(P.price) INTO offer_price FROM products P, configurations C 
                                 WHERE P.pid = C.pid AND C.oid = offer_ID;
            SELECT offer_name, offer_price;

            SELECT P.*, CA.name AS category  
            FROM products P, configurations C, categories CA 
            WHERE C.pid = P.pid AND C.oid = offer_ID AND P.cid = CA.cid;
            
            FETCH c1 INTO offer_ID;
       END WHILE myloop;

CLOSE c1;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `product_delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `product_delete`(IN p_pid INT)
BEGIN

DECLARE num_rec INT;

START TRANSACTION;

SELECT COUNT(*) INTO num_rec FROM order_details WHERE pid = p_pid;
IF num_rec > 0 THEN
  UPDATE products SET deleted = 1 WHERE pid = p_pid;
ELSE
  DELETE FROM products WHERE pid = p_pid;
END IF;

COMMIT;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Current Database: `seng`
--

USE `seng`;

--
-- Final view structure for view `available_products`
--

/*!50001 DROP TABLE IF EXISTS `available_products`*/;
/*!50001 DROP VIEW IF EXISTS `available_products`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `available_products` AS select `products`.`pid` AS `pid`,`products`.`cid` AS `cid`,`products`.`name` AS `name`,`products`.`descr` AS `descr`,`products`.`price` AS `price`,`products`.`availability` AS `availability`,`products`.`deleted` AS `deleted` from `products` where ((`products`.`deleted` = 0) and (`products`.`availability` > 0)) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `catalogue`
--

/*!50001 DROP TABLE IF EXISTS `catalogue`*/;
/*!50001 DROP VIEW IF EXISTS `catalogue`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `catalogue` AS select `p`.`pid` AS `pid`,`p`.`cid` AS `cid`,`p`.`name` AS `name`,`p`.`descr` AS `descr`,`p`.`price` AS `price`,`p`.`availability` AS `availability`,`p`.`deleted` AS `deleted`,`c`.`name` AS `category` from (`products` `p` join `categories` `c` on((`p`.`cid` = `c`.`cid`))) where ((`p`.`availability` > 0) and (`p`.`deleted` = 0)) order by `p`.`pid` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `handled_orders`
--

/*!50001 DROP TABLE IF EXISTS `handled_orders`*/;
/*!50001 DROP VIEW IF EXISTS `handled_orders`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `handled_orders` AS select `O`.`oid` AS `oid`,`O`.`date` AS `date`,`U`.`uid` AS `uid`,concat(`U`.`name`,' ',`U`.`surname`) AS `customer`,`CA`.`name` AS `category`,`P`.`name` AS `product`,`C`.`qty` AS `qty`,`O`.`total` AS `order_total` from ((((`orders` `O` join `order_details` `C`) join `products` `P`) join `categories` `CA`) join `users` `U`) where ((`O`.`oid` = `C`.`oid`) and (`C`.`pid` = `P`.`pid`) and (`P`.`cid` = `CA`.`cid`) and (`O`.`uid` = `U`.`uid`)) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-09-14 15:55:31
