-- phpMyAdmin SQL Dump
-- version 4.1.6
-- http://www.phpmyadmin.net
--
-- Hoszt: 127.0.0.1
-- Létrehozás ideje: 2014. Feb 14. 11:34
-- Szerver verzió: 5.6.15-log
-- PHP verzió: 5.5.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
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
  `activated` tinyint(1) NOT NULL,
  `admin` tinyint(1) NOT NULL,
  `lastOnline` datetime NOT NULL,
  `lang` varchar(5) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- A tábla adatainak kiíratása `accounts`
--

INSERT INTO `accounts` (`id`, `username`, `email`, `password`, `salt`, `activated`, `admin`, `lastOnline`, `lang`) VALUES
(1, 'asd', 'asd', '21e3c338909de5e62f19fa82b0fc3757e648d6f2c36023255bd153c346afc6de6a270a92caefdeca58f0d86e50a892a05a2adb1d09855ae74695a4dd97d31c30', '5cc0da5be3d8d1b6b162cfce1dd43b3d23d3b7c7bfeb727e4f4ecfa429906b233ff342d2d84111e2eab1ee4e2a451f0cc6853e1c05ea2f84d330bbcee1c75dcf', 1, 1, '2014-02-14 11:33:24', 'en');

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `account_activation`
--

CREATE TABLE IF NOT EXISTS `account_activation` (
  `id` int(11) NOT NULL,
  `hash` varchar(32) NOT NULL,
  `sendTime` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  `compError` text NOT NULL,
  `runError` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `accountID` (`accountID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3 ;

--
-- A tábla adatainak kiíratása `bots`
--

INSERT INTO `bots` (`id`, `accountID`, `name`, `lastChangeTime`, `code_lang`, `state`, `compError`, `runError`) VALUES
(1, 1, 'cccccccccccc', '2014-02-13 23:34:36', 'c++', 'ok', '', 0),
(2, 1, 'ddfdfdfdf', '2014-02-14 11:31:23', 'c++', 'ok', '', 0);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `brute_force`
--

CREATE TABLE IF NOT EXISTS `rank_yesterday` (
  `botID` int(11) NOT NULL,
  `rank` int(12) NOT NULL,
  `score` float NOT NULL,
  PRIMARY KEY (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
('ZWIzNTgz', 'addBot', '2014-02-14 12:31:17'),
('ZWIzNTgz', 'addBot', '2014-02-14 12:31:23');

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
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `games_by_bots`
--

CREATE TABLE IF NOT EXISTS `games_by_bots` (
  `gameID` int(11) NOT NULL,
  `botID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `games_by_bots`
--

INSERT INTO `games_by_bots` (`gameID`, `botID`) VALUES
(3, 129);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboard1`
--

CREATE TABLE IF NOT EXISTS `leaderboard1` (
  `botID` int(11) NOT NULL,
  `score` float NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `leaderboard1`
--

INSERT INTO `leaderboard1` (`botID`, `score`, `win`, `loose`) VALUES
(1, 50, 0, 0),
(2, 0, 0, 0);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboard1_yesterday`
--

CREATE TABLE IF NOT EXISTS `leaderboard1_yesterday` (
  `botID` int(11) NOT NULL,
  `score` float NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- A tábla adatainak kiíratása `leaderboard1_yesterday`
--

INSERT INTO `leaderboard1_yesterday` (`botID`, `score`, `win`, `loose`) VALUES
(1, 0, 0, 0),
(2, 200, 0, 0);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboard2`
--

CREATE TABLE IF NOT EXISTS `leaderboard2` (
  `botID` int(11) NOT NULL,
  `score` float NOT NULL,
  `win` int(11) NOT NULL,
  `loose` int(11) NOT NULL,
  PRIMARY KEY (`botID`),
  UNIQUE KEY `botId` (`botID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `leaderboard2_yesterday`
--

CREATE TABLE IF NOT EXISTS `leaderboard2_yesterday` (
  `botID` int(11) NOT NULL,
  `score` float NOT NULL,
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
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=48 ;

--
-- A tábla adatainak kiíratása `news_posts`
--

INSERT INTO `news_posts` (`id`, `title`, `author`, `date`, `content`) VALUES
(1, 'The croupierweb is online!', 'Admin', '2013-09-29 20:38:06', '<h3>Hello!</h3>'),
(46, 'The site is under construction.', 'Admin', '2013-11-10 14:44:22', 'The site is under construction.'),
(47, 'Manual', 'asd', '2014-02-01 16:25:41', 'A manual már elérhető <a href="docs/mi_dev/miDev.html">itt.</a>');

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
('2013-11-06', 1),
('2013-11-08', 3),
('2013-11-10', 1);

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
('2014-02-10', 1),
('2014-02-13', 1),
('2014-02-14', 1);

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
('2013-11-06', 102),
('2013-11-07', 23),
('2013-11-08', 112),
('2013-11-09', 18),
('2013-11-10', 383),
('2013-11-12', 9),
('2013-11-15', 11),
('2014-01-16', 141),
('2014-01-27', 209),
('2014-01-28', 239),
('2014-01-31', 153),
('2014-02-01', 119),
('2014-02-02', 111),
('2014-02-10', 19),
('2014-02-13', 43),
('2014-02-14', 23);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `strings`
--

CREATE TABLE IF NOT EXISTS `strings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `identifier` varchar(30) NOT NULL,
  `language` varchar(5) NOT NULL,
  `text` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=183 ;

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
(99, 'SAVE_BOT_CONF', 'en', 'Are you sure to save?<br />Your bot will be removed from the leaderboards. You can add it again later.'),
(100, 'SAVE_BOT_CONF', 'hu', 'Biztos vagy benne, hogy mented?<br />A botod el lesz távolítva a ranglistákról. Később újból hozzáadhatod.'),
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
(174, 'LEADERBOARD', 'hu', 'Ranglista'),
(175, 'CANCEL_BOT_CONF', 'en', 'The bot won''t be saved.<br />Are you sure to cancel?'),
(176, 'CANCEL_BOT_CONF', 'hu', 'A bot nem lesz mentve.<br />Biztos vagy benne, hogy visszalépsz?'),
(177, 'YES', 'en', 'Yes'),
(178, 'YES', 'hu', 'Igen'),
(179, 'NO', 'en', 'No'),
(180, 'NO', 'hu', 'Nem'),
(181, 'SUMMARY_LATEST_CHANGES', 'en', 'Your bots'' latest rankings'),
(182, 'SUMMARY_LATEST_CHANGES', 'hu', 'A botjaid legutóbbi helyezési');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
