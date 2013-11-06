-- phpMyAdmin SQL Dump
-- version 3.5.8.1
-- http://www.phpmyadmin.net
--
-- Hoszt: 127.0.0.1
-- Létrehozás ideje: 2013. Nov 06. 22:37
-- Szerver verzió: 5.6.12-log
-- PHP verzió: 5.4.14

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Adatbázis: `croupierweb`
--

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `accounts`
--

CREATE TABLE IF NOT EXISTS `accounts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `email` varchar(50) NOT NULL,
  `password` varchar(128) NOT NULL,
  `salt` varchar(128) NOT NULL,
  `admin` tinyint(1) NOT NULL,
  `lastOnline` datetime NOT NULL,
  `language` varchar(5) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

--
-- A tábla adatainak kiíratása `accounts`
--

INSERT INTO `accounts` (`id`, `username`, `email`, `password`, `salt`, `admin`, `lastOnline`, `language`) VALUES
(1, 'asd', 'asd', '60c5603ca9c3a556dc53a260ec30331f38e3fde96b95d3c299c6613cf64732fe07cd86ddb203a2fb3ae52944d5c9fbe333992ba22455db880f736759c1380ce7', '6ef2502227d33bf3fe0ddbb611e4bd3d88893c46a8dfdc6e0942f5abbf40227b51e3bee1dbeede8d524dda4f692d81f812bf5ba02a8e86a938b1de18fba1007c', 1, '2013-11-06 22:26:14', 'hu'),
(2, 'lol', 'lol1', '76f857bbca73a650d211eb2c0c0dc1db282a158111265d68cb16522f880bbb21da688adca02d70cf9abd41e1eea29496d7dddb71402696e5058e2085f7b1c26d', 'c0c7a6d690e8b5ae4352247da7c1609388db2fb2e961e08a75b310565561e3f009ab9d8c6970bb1a59a743af9cbd5e5c021e19989470e1785b71b1cc5bf11ea0', 1, '2013-10-05 20:14:50', 'hu'),
(4, 'lolasd2', 'lol@lol.com', '7df9393aeccdd01c924a39c365ddb08b779d6c65bdf318da15c975156597752d19044dd985d8dd49da34537d4542f82208d2892a777234741de65a0513379c7b', '499cfd4c562d7416e127489611700a08d8671f584e8897760e7b854ad0e1ae0498b6c1217595d218550692d8b9793360877446519105f051a222c0ccc1a65964', 0, '2013-10-18 14:54:50', 'en'),
(5, 'Kulabá', 'sheeeeet@poo.com', 'b1e94bdca8dee012ee0d693ac391e54ebbb54a76530311f34f56a520d6d039bcd7a64deb55cc3c08adf1dfb3191198171bf912be90b9733f1eed4a66ceb655de', '53ef30496db23850d28b072737fc710a10f2567679864899b279790452aa8e1c742150560ba6879715092c5e08d70d347894f4825806a48ee4c6ec498f6bfbd5', 0, '0000-00-00 00:00:00', '');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `bots`
--

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
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=133 ;

--
-- A tábla adatainak kiíratása `bots`
--

INSERT INTO `bots` (`id`, `accountID`, `name`, `lastChangeTime`, `code_lang`, `state`, `ErrorText`) VALUES
(129, 1, 'Unnamed bot 129', '2013-10-22 21:28:34', 'c++', 'ok', ''),
(130, 1, 'probot', '2013-10-22 22:42:37', 'c++', 'ok', ''),
(131, 1, 'pusztito', '2013-10-22 22:42:48', 'c++', 'ok', ''),
(132, 1, 'xcvcv', '2013-10-22 22:43:00', 'c++', 'ok', '');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `brute_force`
--

CREATE TABLE IF NOT EXISTS `brute_force` (
  `id` varchar(8) NOT NULL,
  `action` varchar(30) NOT NULL,
  `expires` datetime NOT NULL,
  KEY `accountID` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `brute_force`
--

INSERT INTO `brute_force` (`id`, `action`, `expires`) VALUES
('NGI4NGIx', 'login', '2013-11-06 22:35:22'),
('NGI4NGIx', 'login', '2013-11-06 22:35:27'),
('NGI4NGIx', 'login', '2013-11-06 22:35:33'),
('NGI4NGIx', 'login', '2013-11-06 22:35:36'),
('NGI4NGIx', 'login', '2013-11-06 22:35:39'),
('NGI4NGIx', 'login', '2013-11-06 22:35:41');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `games`
--

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
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=16 ;

--
-- A tábla adatainak kiíratása `games`
--

INSERT INTO `games` (`id`, `checked`, `leaderboard`, `rules`, `log`, `result`, `startTime`, `endTime`) VALUES
(1, 0, '0', 'lol.txt', '1.log', '1.xml', '2013-10-22 22:35:25', '2013-10-22 22:25:25'),
(2, 0, '0', 'lol.txt', '2.log', '2.xml', '2013-10-22 22:39:28', '2013-10-22 22:29:28'),
(3, 0, '0', 'lol.txt', '3.log', '3.xml', '2013-10-22 22:41:06', '2013-10-22 22:31:06'),
(4, 0, '0', 'testrules.xml', '4.log', '4.xml', '2013-10-22 22:47:00', '2013-10-22 22:37:00'),
(5, 0, '0', 'testrules.xml', '5.log', '5.xml', '2013-10-22 22:47:01', '2013-10-22 22:37:01'),
(6, 0, '0', 'testrules.xml', '6.log', '6.xml', '2013-10-22 22:50:19', '2013-10-22 22:40:19'),
(7, 0, '0', 'testrules.xml', '7.log', '7.xml', '2013-10-22 22:50:42', '2013-10-22 22:40:42'),
(8, 0, '0', 'testrules.xml', '8.log', '8.xml', '2013-10-22 22:50:44', '2013-10-22 22:40:44'),
(9, 0, '0', 'testrules.xml', '9.log', '9.xml', '2013-10-22 22:56:50', '2013-10-22 22:46:50'),
(10, 0, '0', 'testrules.xml', '10.log', '10.xml', '2013-10-22 22:58:12', '2013-10-22 22:48:12'),
(11, 0, '0', 'testrules.xml', '11.log', '11.xml', '2013-10-22 22:58:49', '2013-10-22 22:48:49'),
(12, 0, '0', 'testrules.xml', '12.log', '12.xml', '2013-10-22 23:04:41', '0000-00-00 00:00:00'),
(13, 0, '0', 'testrules.xml', '13.log', '13.xml', '2013-10-22 23:07:06', '0000-00-00 00:00:00'),
(14, 0, 'leaderboard1', 'testrules.xml', '14.log', '14.xml', '2013-10-22 23:19:08', '2013-10-22 23:09:08'),
(15, 0, 'leaderboard1', 'testrules.xml', '15.log', '15.xml', '2013-10-22 23:19:21', '2013-10-22 23:09:21');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboard1`
--

CREATE TABLE IF NOT EXISTS `leaderboard1` (
  `botID` int(11) NOT NULL,
  `score` int(11) NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `leaderboard1`
--

INSERT INTO `leaderboard1` (`botID`, `score`, `win`, `loose`) VALUES
(129, 0, 0, 0),
(130, 0, 0, 0),
(131, 0, 0, 0),
(132, 0, 0, 0);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboard2`
--

CREATE TABLE IF NOT EXISTS `leaderboard2` (
  `botID` int(11) NOT NULL,
  `score` int(11) NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboards`
--

CREATE TABLE IF NOT EXISTS `leaderboards` (
  `tableName` varchar(20) NOT NULL,
  `rules` varchar(50) NOT NULL,
  `activated` tinyint(1) NOT NULL,
  `lastRefresh` datetime NOT NULL,
  PRIMARY KEY (`tableName`),
  UNIQUE KEY `id` (`tableName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `leaderboards`
--

INSERT INTO `leaderboards` (`tableName`, `rules`, `activated`, `lastRefresh`) VALUES
('leaderboard1', 'testrules.xml', 1, '2013-10-22 23:19:21'),
('leaderboard2', 'testrules.xml', 1, '2013-10-22 22:47:01');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `news_posts`
--

CREATE TABLE IF NOT EXISTS `news_posts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(200) NOT NULL,
  `author` varchar(50) NOT NULL,
  `date` datetime NOT NULL,
  `content` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=46 ;

--
-- A tábla adatainak kiíratása `news_posts`
--

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
(45, 'SEnd moooorrrreeee bots!', 'asd', '2013-10-19 16:27:04', 'Plese send more <b>BOOOOTSSSS</b>!');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `stat_accounts_added`
--

CREATE TABLE IF NOT EXISTS `stat_accounts_added` (
  `date` date NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `stat_accounts_added`
--

INSERT INTO `stat_accounts_added` (`date`, `count`) VALUES
('2013-10-02', 2),
('2013-10-04', 4),
('2013-10-06', 7),
('2013-10-07', 2),
('2013-11-06', 1);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `stat_bots_added`
--

CREATE TABLE IF NOT EXISTS `stat_bots_added` (
  `date` date NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `stat_bots_added`
--

INSERT INTO `stat_bots_added` (`date`, `count`) VALUES
('2013-09-29', 3),
('2013-10-01', 0),
('2013-10-04', 3),
('2013-10-05', 3),
('2013-10-19', 2),
('2013-10-21', 1),
('2013-10-22', 22);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `stat_pageload`
--

CREATE TABLE IF NOT EXISTS `stat_pageload` (
  `date` date NOT NULL,
  `count` int(11) NOT NULL,
  PRIMARY KEY (`date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `stat_pageload`
--

INSERT INTO `stat_pageload` (`date`, `count`) VALUES
('2013-10-19', 263),
('2013-10-20', 352),
('2013-10-21', 57),
('2013-10-22', 148),
('2013-11-06', 102);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `strings`
--

CREATE TABLE IF NOT EXISTS `strings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `identifier` varchar(30) NOT NULL,
  `language` varchar(5) NOT NULL,
  `text` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=127 ;

--
-- A tábla adatainak kiíratása `strings`
--

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
(19, 'REG_THANKS', 'en', 'Thank you for registering!'),
(20, 'REG_THANKS', 'hu', 'Köszönjük a regisztrációt!'),
(21, 'NEW_BOT', 'en', 'New bot'),
(22, 'NEW_BOT', 'hu', 'Új bot'),
(23, 'MANAGE_BOTS', 'en', 'Manage bots'),
(24, 'MANAGE_BOTS', 'hu', 'Botok kezelése'),
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
(87, 'ERR_CODE_EMPTY', 'en', 'The program code is too short!'),
(88, 'ERR_CODE_EMPTY', 'hu', 'A program kód túl rövid!'),
(89, 'ERR_USERNAME_EXITS', 'en', 'The username already exits. Choose an other one!'),
(90, 'ERR_USERNAME_EXITS', 'hu', 'A felhasználónév már foglalt. Válassz egy másikat!'),
(91, 'UNNAMED_BOT', 'en', 'Unnamed bot'),
(92, 'UNNAMED_BOT', 'hu', 'Névtelen bot'),
(93, 'ACCOUNT_MANAGEMENT', 'en', 'Account management'),
(94, 'ACCOUNT_MANAGEMENT', 'hu', 'Fiókkezelés'),
(95, 'EDIT_BOT', 'en', 'Edit bot'),
(96, 'EDIT_BOT', 'hu', 'Bot szerkesztése'),
(97, 'SAVE', 'en', 'Save'),
(98, 'SAVE', 'hu', 'Mentés'),
(99, 'SAVE_BOT_CONF', 'en', 'Are you sure to save?\\nYour bot will be removed from the leaderboards.'),
(100, 'SAVE_BOT_CONF', 'hu', 'Biztos vagy benne, hogy mented?\\nA botod el lesz távolítva a ranglistákról.'),
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
(111, 'LOADING', 'en', 'Loading...'),
(112, 'LOADING', 'hu', 'Betöltés...'),
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
(124, 'CAPTCHA', 'hu', 'Kérlek oldd meg az ellenőrző mezőt');

--
-- Megkötések a kiírt táblákhoz
--

--
-- Megkötések a táblához `bots`
--
ALTER TABLE `bots`
  ADD CONSTRAINT `bots_ibfk_1` FOREIGN KEY (`accountID`) REFERENCES `accounts` (`id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
