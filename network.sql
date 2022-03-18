-- MySQL dump 10.13  Distrib 8.0.27, for Linux (x86_64)
--
-- Host: localhost    Database: network
-- ------------------------------------------------------
-- Server version	8.0.27-0ubuntu0.20.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `network_group`
--

DROP TABLE IF EXISTS `network_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `network_group` (
  `GroupID` int NOT NULL AUTO_INCREMENT,
  `GroupName` varchar(255) NOT NULL,
  `GroupDescription` varchar(255) NOT NULL,
  `UserID` int NOT NULL,
  PRIMARY KEY (`GroupID`),
  KEY `UserID` (`UserID`),
  CONSTRAINT `network_group_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `network_user` (`UserID`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `network_group`
--

LOCK TABLES `network_group` WRITE;
/*!40000 ALTER TABLE `network_group` DISABLE KEYS */;
INSERT INTO `network_group` VALUES (1,'Nhom1','deptrai wa',1);
/*!40000 ALTER TABLE `network_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `network_user`
--

DROP TABLE IF EXISTS `network_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `network_user` (
  `UserID` int NOT NULL AUTO_INCREMENT,
  `UserName` varchar(255) NOT NULL,
  `Email` varchar(255) NOT NULL,
  `Password` varchar(255) NOT NULL,
  PRIMARY KEY (`UserID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `network_user`
--

LOCK TABLES `network_user` WRITE;
/*!40000 ALTER TABLE `network_user` DISABLE KEYS */;
INSERT INTO `network_user` VALUES (1,'haha','haha@gmail.com','haha'),(2,'admin','admin@gmail.com','admin');
/*!40000 ALTER TABLE `network_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `share_file`
--

DROP TABLE IF EXISTS `share_file`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `share_file` (
  `FileID` int NOT NULL AUTO_INCREMENT,
  `FileName` varchar(255) NOT NULL,
  `UserID` int NOT NULL,
  `FolderID` int NOT NULL,
  PRIMARY KEY (`FileID`),
  KEY `UserID` (`UserID`),
  KEY `FolderID` (`FolderID`),
  CONSTRAINT `share_file_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `network_user` (`UserID`) ON DELETE CASCADE,
  CONSTRAINT `share_file_ibfk_2` FOREIGN KEY (`FolderID`) REFERENCES `share_folder` (`FolderID`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `share_file`
--

LOCK TABLES `share_file` WRITE;
/*!40000 ALTER TABLE `share_file` DISABLE KEYS */;
INSERT INTO `share_file` VALUES (1,'Apocalypse.mp4',1,1),(2,'client.c',2,1);
/*!40000 ALTER TABLE `share_file` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `share_folder`
--

DROP TABLE IF EXISTS `share_folder`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `share_folder` (
  `FolderID` int NOT NULL AUTO_INCREMENT,
  `FolderName` varchar(255) NOT NULL,
  `UserID` int NOT NULL,
  `GroupID` int NOT NULL,
  PRIMARY KEY (`FolderID`),
  KEY `UserID` (`UserID`),
  KEY `GroupID` (`GroupID`),
  CONSTRAINT `share_folder_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `network_user` (`UserID`) ON DELETE CASCADE,
  CONSTRAINT `share_folder_ibfk_2` FOREIGN KEY (`GroupID`) REFERENCES `network_group` (`GroupID`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `share_folder`
--

LOCK TABLES `share_folder` WRITE;
/*!40000 ALTER TABLE `share_folder` DISABLE KEYS */;
INSERT INTO `share_folder` VALUES (1,'root',1,1);
/*!40000 ALTER TABLE `share_folder` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `share_group`
--

DROP TABLE IF EXISTS `share_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `share_group` (
  `UserID` int NOT NULL,
  `GroupID` int NOT NULL,
  KEY `UserID` (`UserID`),
  KEY `GroupID` (`GroupID`),
  CONSTRAINT `share_group_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `network_user` (`UserID`) ON DELETE CASCADE,
  CONSTRAINT `share_group_ibfk_2` FOREIGN KEY (`GroupID`) REFERENCES `network_group` (`GroupID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `share_group`
--

LOCK TABLES `share_group` WRITE;
/*!40000 ALTER TABLE `share_group` DISABLE KEYS */;
INSERT INTO `share_group` VALUES (1,1),(2,1);
/*!40000 ALTER TABLE `share_group` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-14 22:03:40
