SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for rep_system
-- ----------------------------
DROP TABLE IF EXISTS `rep_system`;
CREATE TABLE `rep_system` (
  `account` int(11) NOT NULL,
  `points` int(10) DEFAULT NULL,
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=cp1250;

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for rep_system_check
-- ----------------------------
DROP TABLE IF EXISTS `rep_system_check`;
CREATE TABLE `rep_system_check` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `sender` int(11) NOT NULL,
  `receiver` int(11) NOT NULL,
  `date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=cp1250;