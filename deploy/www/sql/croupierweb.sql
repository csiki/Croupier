SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;


CREATE TABLE IF NOT EXISTS `accounts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `email` varchar(50) NOT NULL,
  `password` varchar(128) NOT NULL,
  `salt` varchar(128) NOT NULL,
  `activated` tinyint(1) NOT NULL,
  `admin` tinyint(1) NOT NULL,
  `lastOnline` datetime NOT NULL,
  `lang` varchar(5) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=17 ;

INSERT INTO `accounts` (`id`, `username`, `email`, `password`, `salt`, `activated`, `admin`, `lastOnline`, `lang`) VALUES
(1, 'asd', 'asd', '21e3c338909de5e62f19fa82b0fc3757e648d6f2c36023255bd153c346afc6de6a270a92caefdeca58f0d86e50a892a05a2adb1d09855ae74695a4dd97d31c30', '5cc0da5be3d8d1b6b162cfce1dd43b3d23d3b7c7bfeb727e4f4ecfa429906b233ff342d2d84111e2eab1ee4e2a451f0cc6853e1c05ea2f84d330bbcee1c75dcf', 1, 1, '2014-01-28 19:33:20', 'hu'),
(2, 'lol', 'lol1', '76f857bbca73a650d211eb2c0c0dc1db282a158111265d68cb16522f880bbb21da688adca02d70cf9abd41e1eea29496d7dddb71402696e5058e2085f7b1c26d', 'c0c7a6d690e8b5ae4352247da7c1609388db2fb2e961e08a75b310565561e3f009ab9d8c6970bb1a59a743af9cbd5e5c021e19989470e1785b71b1cc5bf11ea0', 1, 1, '2013-10-05 20:14:50', 'hu'),
(4, 'lolasd2', 'lol@lol.com', '7df9393aeccdd01c924a39c365ddb08b779d6c65bdf318da15c975156597752d19044dd985d8dd49da34537d4542f82208d2892a777234741de65a0513379c7b', '499cfd4c562d7416e127489611700a08d8671f584e8897760e7b854ad0e1ae0498b6c1217595d218550692d8b9793360877446519105f051a222c0ccc1a65964', 1, 0, '2013-10-18 14:54:50', 'en'),
(5, 'Kulabá', 'sheeeeet@poo.com', 'b1e94bdca8dee012ee0d693ac391e54ebbb54a76530311f34f56a520d6d039bcd7a64deb55cc3c08adf1dfb3191198171bf912be90b9733f1eed4a66ceb655de', '53ef30496db23850d28b072737fc710a10f2567679864899b279790452aa8e1c742150560ba6879715092c5e08d70d347894f4825806a48ee4c6ec498f6bfbd5', 0, 0, '0000-00-00 00:00:00', ''),
(16, 'Ruzar', 'ruzar@mail.com', 'b64d9fa7773d8535b422cde6d79c6945edc4d4220b40cd24bac574c13a69300d6193bc510ae253bf4d5d4163ea0c542e679055d8b544c6d1a17c9e5949929a35', 'b1884c114492f68fbbefdffb144af2ad9ab2b800835ec090f3829ff0dfdebdd24dd77f3af44dc3f64f175ef5c3f8bc8ed50fde98ecc466eb7ee23a8d695c785c', 1, 0, '2013-11-08 14:17:13', 'hu');

CREATE TABLE IF NOT EXISTS `account_activation` (
  `id` int(11) NOT NULL,
  `hash` varchar(32) NOT NULL,
  `sendTime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `account_activation` (`id`, `hash`, `sendTime`) VALUES
(17, '3def184ad8f4755ff269862ea77393dd', '2013-11-10 11:47:22');

CREATE TABLE IF NOT EXISTS `bots` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `accountID` int(12) NOT NULL,
  `name` varchar(30) NOT NULL,
  `lastChangeTime` datetime NOT NULL,
  `code_lang` enum('c++','java','c#') NOT NULL,
  `state` enum('processing','ok','runtime','compilation') NOT NULL,
  `ErrorText` varchar(50) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `accountID` (`accountID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=139 ;

INSERT INTO `bots` (`id`, `accountID`, `name`, `lastChangeTime`, `code_lang`, `state`, `ErrorText`) VALUES
(129, 1, 'Unnamed bot 129', '2014-01-28 15:45:36', 'c++', 'processing', ''),
(130, 1, 'probot', '2013-10-22 22:42:37', 'c++', 'ok', ''),
(131, 1, 'pusztito', '2013-10-22 22:42:48', 'c++', 'ok', ''),
(132, 1, 'xcvcv', '2013-10-22 22:43:00', 'c++', 'ok', ''),
(133, 1, 'sfdsdf', '2013-11-12 23:07:10', 'c++', 'processing', '');

CREATE TABLE IF NOT EXISTS `brute_force` (
  `id` varchar(8) NOT NULL,
  `action` varchar(30) NOT NULL,
  `expires` datetime NOT NULL,
  KEY `accountID` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `brute_force` (`id`, `action`, `expires`) VALUES
('NGI4NGIx', 'login', '2014-01-28 19:37:30');

CREATE TABLE IF NOT EXISTS `games` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `checked` tinyint(1) NOT NULL,
  `leaderboard` text NOT NULL,
  `rules` varchar(50) NOT NULL,
  `log` varchar(50) NOT NULL,
  `result` varchar(50) NOT NULL,
  `startTime` datetime NOT NULL,
  `endTime` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=28 ;

INSERT INTO `games` (`id`, `checked`, `leaderboard`, `rules`, `log`, `result`, `startTime`, `endTime`) VALUES
(1, 0, 'leaderboard1', 'lol.txt', '1.log', '1.xml', '2013-10-22 22:35:25', '2013-10-22 22:25:25'),
(2, 0, 'leaderboard1', 'lol.txt', '2.log', '2.xml', '2013-10-22 22:39:28', '2013-10-22 22:29:28'),
(3, 1, 'leaderboard1', 'lol.txt', 'testlog.xml', '3.xml', '2013-10-22 22:41:06', '2013-10-22 22:31:06'),
(4, 0, 'leaderboard1', 'testrules.xml', '4.log', '4.xml', '2013-10-22 22:47:00', '2013-10-22 22:37:00'),
(5, 0, 'leaderboard1', 'testrules.xml', '5.log', '5.xml', '2013-10-22 22:47:01', '2013-10-22 22:37:01'),
(6, 0, 'leaderboard2', 'testrules.xml', '6.log', '6.xml', '2013-10-22 22:50:19', '2013-10-22 22:40:19'),
(7, 0, 'leaderboard2', 'testrules.xml', '7.log', '7.xml', '2013-10-22 22:50:42', '2013-10-22 22:40:42'),
(8, 0, 'leaderboard2', 'testrules.xml', '8.log', '8.xml', '2013-10-22 22:50:44', '2013-10-22 22:40:44'),
(9, 0, 'leaderboard2', 'testrules.xml', '9.log', '9.xml', '2013-10-22 22:56:50', '2013-10-22 22:46:50'),
(10, 0, 'leaderboard2', 'testrules.xml', '10.log', '10.xml', '2013-10-22 22:58:12', '2013-10-22 22:48:12'),
(11, 0, 'leaderboard2', 'testrules.xml', '11.log', '11.xml', '2013-10-22 22:58:49', '2013-10-22 22:48:49');

CREATE TABLE IF NOT EXISTS `games_by_bots` (
  `gameID` int(11) NOT NULL,
  `botID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `games_by_bots` (`gameID`, `botID`) VALUES
(3, 129);

CREATE TABLE IF NOT EXISTS `leaderboard1` (
  `botID` int(11) NOT NULL,
  `score` float NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `leaderboard1` (`botID`, `score`, `win`, `loose`) VALUES
(130, 0, 0, 0),
(131, 0, 0, 0),
(132, 0, 0, 0);

CREATE TABLE IF NOT EXISTS `leaderboard2` (
  `botID` int(11) NOT NULL,
  `score` float NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `leaderboards` (
  `tableName` varchar(20) NOT NULL,
  `rules` varchar(50) NOT NULL,
  `activated` tinyint(1) NOT NULL,
  `lastRefresh` datetime NOT NULL,
  PRIMARY KEY (`tableName`),
  UNIQUE KEY `id` (`tableName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `leaderboards` (`tableName`, `rules`, `activated`, `lastRefresh`) VALUES
('leaderboard1', 'testrules.xml', 1, '2013-10-22 23:19:21'),
('leaderboard2', 'testrules.xml', 1, '2013-10-22 22:47:01');

CREATE TABLE IF NOT EXISTS `news_posts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(200) NOT NULL,
  `author` varchar(50) NOT NULL,
  `date` datetime NOT NULL,
  `content` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=47 ;

INSERT INTO `news_posts` (`id`, `title`, `author`, `date`, `content`) VALUES
(1, 'The croupierweb is online!', 'Admin', '2013-09-29 20:38:06', '<h3>Hello!</h3>\r\n<br />\r\nHappy to announce, this is the very first post!!4!4!!'),
(2, 'aaaa', 'sdf', '2013-09-29 20:48:18', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(3, 'aaaa', 'sdf', '2013-09-29 20:49:05', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(4, 'aaaa', 'sdf', '2013-09-29 20:49:05', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(5, 'aaaa', 'sdf', '2013-09-29 20:49:05', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(6, 'aaaa', 'sdf', '2013-09-29 20:49:05', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(7, 'aaaa', 'sdf', '2013-09-29 20:49:05', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(8, 'aaaa', 'sdf', '2013-09-29 20:49:05', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(9, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(10, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(11, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(12, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(13, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(14, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(15, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(16, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(17, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(18, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(19, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(20, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(21, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(22, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(23, 'aaaa', 'sdf', '2013-09-29 20:49:06', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(24, 'aaaa', 'sdf', '2013-09-29 20:49:07', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(25, 'aaaa', 'sdf', '2013-09-29 20:49:07', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(26, 'aaaa', 'sdf', '2013-09-29 20:49:07', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(27, 'aaaa', 'sdf', '2013-09-29 20:49:07', 'akfmlkfmsdlfmsdlmfsdlkfslksmflksmflksmflsmdlfksf'),
(45, 'SEnd moooorrrreeee bots!', 'asd', '2013-10-19 16:27:04', 'Plese send more <b>BOOOOTSSSS</b>!'),
(46, 'The site is under construction.', 'asd', '2013-11-10 14:44:22', 'The site is under construction.');

CREATE TABLE IF NOT EXISTS `stat_accounts_added` (
  `date` date NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `stat_accounts_added` (`date`, `count`) VALUES
('2013-10-02', 2),
('2013-10-04', 4),
('2013-10-06', 7),
('2013-10-07', 2),
('2013-11-06', 1),
('2013-11-08', 3),
('2013-11-10', 1);

CREATE TABLE IF NOT EXISTS `stat_bots_added` (
  `date` date NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `stat_bots_added` (`date`, `count`) VALUES
('2013-09-29', 3),
('2013-10-01', 0),
('2013-10-04', 3),
('2013-10-05', 3),
('2013-10-19', 2),
('2013-10-21', 1),
('2013-10-22', 22),
('2013-11-07', 2),
('2013-11-12', 1),
('2014-01-27', 5);

CREATE TABLE IF NOT EXISTS `stat_pageload` (
  `date` date NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `stat_pageload` (`date`, `count`) VALUES
('2013-10-19', 263),
('2013-10-20', 352),
('2013-10-21', 57),
('2013-10-22', 148),
('2013-11-06', 102),
('2013-11-07', 23),
('2013-11-08', 112),
('2013-11-09', 18),
('2013-11-10', 383),
('2013-11-12', 9),
('2013-11-15', 11),
('2014-01-16', 141),
('2014-01-27', 209),
('2014-01-28', 181);

CREATE TABLE IF NOT EXISTS `strings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `identifier` varchar(30) NOT NULL,
  `language` varchar(5) NOT NULL,
  `text` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=175 ;

INSERT INTO `strings` (`id`, `identifier`, `language`, `text`) VALUES
(1, 'WEBPAGENAME', 'hu', 'Croupier poker AI'),
(2, 'WEBPAGENAME', 'en', 'Croupier poker AI'),
(3, 'SUMMARY', 'en', 'Summary'),
(4, 'SUMMARY', 'hu', 'Összefogaló'),
(5, 'MAIN', 'en', 'Main'),
(6, 'MAIN', 'hu', 'Főoldal'),
(11, 'LOGOUT', 'en', 'Log out'),
(12, 'LOGOUT', 'hu', 'Kijelentkezés'),
(13, 'LOGIN', 'en', 'Log in'),
(14, 'LOGIN', 'hu', 'Bejelentkezés'),
(15, 'SUMMARY_WELCOME', 'en', 'Welcome to the members area!'),
(16, 'SUMMARY_WELCOME', 'hu', 'Üdvözöllek!'),
(17, 'REGISTER', 'en', 'Register'),
(18, 'REGISTER', 'hu', 'Regisztrálás'),
(19, 'REGISTER_THANKS', 'en', 'Thank you for registering! We''ve sent an email to %s. Open it up to activate your account.'),
(20, 'REGISTER_THANKS', 'hu', 'Köszönjük a regisztrációt! Küldtünk egy megerősítő e-mailt. Nyisd meg, hogy aktiváld a fiókod.'),
(21, 'NEW_BOT', 'en', 'New bot'),
(22, 'NEW_BOT', 'hu', 'Új bot'),
(23, 'MY_BOTS', 'en', 'My bots'),
(24, 'MY_BOTS', 'hu', 'Saját botok'),
(25, 'NAME', 'en', 'Name'),
(26, 'NAME', 'hu', 'Név'),
(27, 'DATE_TIME', 'en', 'Date/Time'),
(28, 'DATE_TIME', 'hu', 'Dátum/Idő'),
(29, 'CODE_LANG', 'en', 'Language'),
(30, 'CODE_LANG', 'hu', 'Nyelv'),
(31, 'STATE', 'en', 'State'),
(32, 'STATE', 'hu', 'Állapot'),
(33, 'PASSWORD', 'en', 'Password'),
(34, 'PASSWORD', 'hu', 'Jelszó'),
(35, 'REG_ACCOUNT_TITLE', 'en', 'Register new account'),
(36, 'REG_ACCOUNT_TITLE', 'hu', 'Új fiók létrehozása'),
(37, 'ERR_USERNAME_LENGTH', 'en', 'The usernamame must be betweeen 3 and 25 characters!'),
(38, 'ERR_USERNAME_LENGTH', 'hu', 'A felhasználónévnek 3 és 25 karakter között kell lennie!'),
(39, 'USERNAME', 'en', 'Username'),
(40, 'USERNAME', 'hu', 'Felhasználónév'),
(41, 'ERR_PASSWORD_LENGTH', 'en', 'The password must be betweeen 6 and 100 characters!'),
(42, 'ERR_PASSWORD_LENGTH', 'hu', 'A jelszónak 6 és 100 karakter között kell lennie!'),
(43, 'ERR_EMAIL', 'en', 'The email address is incorrect!'),
(44, 'ERR_EMAIL', 'hu', 'Az email cím helytelen!'),
(47, 'ERR_LOGIN', 'en', 'The given email address or password is incorrect!'),
(48, 'ERR_LOGIN', 'hu', 'A megadott email cím vagy jelszó helytelen!'),
(49, 'LEADERBOARDS', 'en', 'Leaderboards'),
(50, 'LEADERBOARDS', 'hu', 'Ranglisták'),
(51, 'LEADERBOARDS', 'en', 'Leaderboards'),
(52, 'LEADERBOARDS', 'hu', 'Ranglisták'),
(53, 'NEWER_PAGES', 'hu', 'Újabb bejegyzések'),
(54, 'NEWER_PAGES', 'en', 'Newer posts'),
(55, 'OLDER_PAGES', 'hu', 'Régebbi bejegyzések'),
(56, 'OLDER_PAGES', 'en', 'Older posts'),
(57, 'NO_BOTS_FOUND', 'en', 'No bots found.'),
(58, 'NO_BOTS_FOUND', 'hu', 'Nem található bot.'),
(61, 'INSERT_CODE', 'en', 'Insert code here'),
(62, 'INSERT_CODE', 'hu', 'Írd ide a kódot'),
(63, 'CHOOSE_FILE_TO', 'en', 'OR choose a file to upload'),
(64, 'CHOOSE_FILE_TO', 'hu', 'VAGY tölts fel egy fájlt'),
(65, 'CODE_LANG', 'en', 'Language'),
(66, 'CODE_LANG', 'hu', 'Nyelv'),
(67, 'SUBMIT', 'en', 'Submit'),
(68, 'SUBMIT', 'hu', 'Elküld'),
(69, 'BOTNAME', 'en', 'Bot name'),
(70, 'BOTNAME', 'hu', 'Bot név'),
(71, 'ERR_CODEFILE', 'en', 'Invalid file!'),
(72, 'ERR_CODEFILE', 'hu', 'Helytelen fájl!'),
(73, 'DELETE_BOT_CONF', 'en', 'Are you sure to delete bot?'),
(74, 'DELETE_BOT_CONF', 'hu', 'Biztos, hogy eltávolítod a botot?'),
(87, 'ERR_CODE_SHORT', 'en', 'The code is too short!'),
(88, 'ERR_CODE_SHORT', 'hu', 'A kód túl rövid!'),
(89, 'ERR_USERNAME_EXITS', 'en', 'The username already exits. Choose an other one!'),
(90, 'ERR_USERNAME_EXITS', 'hu', 'A felhasználónév már foglalt. Válassz egy másikat!'),
(91, 'UNNAMED_BOT', 'en', 'Unnamed bot'),
(92, 'UNNAMED_BOT', 'hu', 'Névtelen bot'),
(95, 'EDIT_BOT', 'en', 'Edit bot'),
(96, 'EDIT_BOT', 'hu', 'Bot szerkesztése'),
(97, 'SAVE', 'en', 'Save'),
(98, 'SAVE', 'hu', 'Mentés'),
(99, 'SAVE_BOT_CONF', 'en', 'Are you sure to save?\\nYour bot will be removed from the leaderboards. You can add it again later.'),
(100, 'SAVE_BOT_CONF', 'hu', 'Biztos vagy benne, hogy mented?\\nA botod el lesz távolítva a ranglistákról. Később újból hozzáadhatod.'),
(101, 'ACCOUNT_SAVED', 'en', 'Your changes have been saved.'),
(102, 'ACCOUNT_SAVED', 'hu', 'A változtatásaid elmentve.'),
(103, 'OPERATIONS', 'en', 'Operations'),
(104, 'OPERATIONS', 'hu', 'Műveletek'),
(105, 'SCORE', 'en', 'Score'),
(106, 'SCORE', 'hu', 'Pontok'),
(107, 'PARTICIPATE', 'en', 'Participate'),
(108, 'PARTICIPATE', 'hu', 'Nevezés'),
(109, 'BACKTRACK', 'en', 'Backtrack'),
(110, 'BACKTRACK', 'hu', 'Visszalépés'),
(111, 'PROCESSING', 'en', 'Processing...'),
(112, 'PROCESSING', 'hu', 'Feldolgozás...'),
(113, 'DELETE_BOT', 'en', 'Delete bot'),
(114, 'DELETE_BOT', 'hu', 'Bot törlése'),
(115, 'CANCEL', 'en', 'Cancel'),
(116, 'CANCEL', 'hu', 'Mégse'),
(117, 'BACK', 'en', 'Back'),
(118, 'BACK', 'hu', 'Vissza'),
(119, 'ERR_NAME_CONFLICT', 'en', 'The name already exits.'),
(120, 'ERR_NAME_CONFLICT', 'hu', 'A név már létezik.'),
(121, 'ERR_CAPTCHA', 'en', 'The entered text does not match the image.'),
(122, 'ERR_CAPTCHA', 'hu', 'A megadott szöveg nem egyezik a képpel.'),
(123, 'CAPTCHA', 'en', 'Please solve the captcha'),
(124, 'CAPTCHA', 'hu', 'Kérlek oldd meg az ellenőrző mezőt'),
(127, 'ERR_ADDBOT_BRUTE', 'en', 'You can only submit 30 bots per hour.'),
(128, 'ERR_ADDBOT_BRUTE', 'hu', 'Óránként csak 30 botot küldhetsz be.'),
(129, 'REGISTERED_EMAIL_TITLE', 'en', 'Welcome to Croupier poker framework'),
(130, 'REGISTERED_EMAIL_TITLE', 'hu', 'Üdvözlünk a Croupier poker framework-ön'),
(131, 'REGISTERED_EMAIL_MESSAGE', 'en', 'Hello,\r\n\r\nThis message was sent to you at your request to sign up in Croupier poker framework.\r\n \r\nIf you want to register, you will need to click the link below and complete registration:\r\n \r\n%s\r\n \r\nIf you do not want to complete the registration, ignore this message or delete it.\r\n \r\nThank you,\r\nCroupier developers'),
(132, 'REGISTERED_EMAIL_MESSAGE', 'hu', 'Hello,\r\n\r\nEzt az üzeneted azért kapod, mert regisztráltál a Croupier poker framework-re.\r\n \r\nHogy aktiváld a fiókod, kattints erre a linkre:\r\n \r\n%s\r\n \r\nHa nem akarod befejezni a regisztrációt, hagyd figyelmen kívül ezt a levelet, vagy töröld.\r\n \r\nKöszönjük,\r\nCroupier developers'),
(133, 'ACCOUNT_ALREADY_ACTIVATED', 'en', 'Your account is already activated. You can log in right now!'),
(134, 'ACCOUNT_ALREADY_ACTIVATED', '', 'A fiókod már aktiválva van. Azonnal bejelentkezhetsz!'),
(135, 'ACCOUNT_ACTIVATED', 'en', 'Your account is successfully activated. Log in right now!'),
(136, 'ACCOUNT_ACTIVATED', 'hu', 'A fiókodat sikeresen aktiváltuk. Azonnal bejelentkezhetsz!'),
(137, 'ACCOUNT_NOT_FOUND', 'en', 'Sorry, we can not find your account.'),
(138, 'ACCOUNT_NOT_FOUND', 'hu', 'Sajnáljuk, nem találjuk a fiókod.'),
(139, 'ACCOUNT_HASH_ERR', 'en', 'Your account can not be activated. Make sure you have typed the correct address!'),
(140, 'ACCOUNT_HASH_ERR', 'hu', 'A fiókod nem lehet aktiválni. Ellenőrizd, hogy a helyes címet írtad be!'),
(141, 'LOGIN_ACTIVATION_ERR', 'en', 'Your account is not yet activated. Please check your e-mail.'),
(142, 'LOGIN_ACTIVATION_ERR', 'hu', 'A fiókod még nincs aktiválva. Kérjük ellenőrizd az e-mail fiókod.'),
(143, 'OWNER', 'en', 'Owner'),
(144, 'OWNER', 'hu', 'Tulajdonos'),
(145, 'PASSWORD_AGAIN', 'en', 'Password again'),
(146, 'PASSWORD_AGAIN', 'hu', 'Jelszó még egyszer'),
(147, 'PASS_NOT_MATCH', 'en', 'The passwords does not match!'),
(148, 'PASS_NOT_MATCH', 'hu', 'A jelszavak nem egyeznek!'),
(149, 'FOOTER_TEXT', 'en', 'Got questions? You can find us at %s'),
(150, 'FOOTER_TEXT', 'hu', 'Kérdéseid vannak? Megtalálsz minket a %s címen.'),
(151, 'EDIT_ACCOUNT', 'en', 'Edit account'),
(152, 'EDIT_ACCOUNT', 'hu', 'Fiók módosítása'),
(153, 'PARTICIPATED_BOTS', 'en', 'Participated bots'),
(154, 'PARTICIPATED_BOTS', 'hu', 'Nevezett botok'),
(155, 'LOADING', 'en', 'Loading...'),
(156, 'LOADING', 'hu', 'Betöltés...'),
(157, 'PLAYED_GAMES_FOR', 'en', 'Played games for %s'),
(158, 'PLAYED_GAMES_FOR', 'hu', '%s lejátszott játékai'),
(159, 'PLAYED_GAMES', 'en', 'Played games'),
(160, 'PLAYED_GAMES', 'hu', 'Lejátszott játékok'),
(161, 'NO_GAMES_FOUND', 'en', 'No games found'),
(162, 'NO_GAMES_FOUND', 'hu', 'Nem találhatóak játékok'),
(163, 'SHOW', 'en', 'Show'),
(164, 'SHOW', 'hu', 'Megmutat'),
(165, 'SHOW_GAME', 'en', '%s''s game at %s'),
(166, 'SHOW_GAME', 'hu', '%s játéka %s-kor'),
(167, 'ERR_USERNAME_FORMAT', 'en', 'The username can only contain the following characters: a-z, A-Z, 0-9!'),
(168, 'ERR_USERNAME_FORMAT', 'hu', 'A felhasználónév csak a következő karaktereket tartalmazhatja: a-z, A-Z, 0-9!'),
(169, 'ERR_BOTNAME_LENGTH', 'en', 'Bot name must be between 5 and 30 characters!'),
(170, 'ERR_BOTNAME_LENGTH', 'hu', 'A bot nevének 5 és 30 karakter között kell lennie!'),
(171, 'ERR_BOTNAME_FORMAT', 'en', 'The bot name can only contain the following characters: a-z, A-Z, 0-9!'),
(172, 'ERR_BOTNAME_FORMAT', 'hu', 'A bot neve csak a következő karaktereket tartalmazhatja: a-z, A-Z, 0-9!'),
(173, 'LEADERBOARD', 'en', 'Leaderboard'),
(174, 'LEADERBOARD', 'hu', 'Ranglista');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
