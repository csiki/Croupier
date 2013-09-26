CREATE DATABASE `croupierweb`
  DEFAULT CHARACTER SET utf8
  COLLATE utf8;

CREATE TABLE `accounts` (
  `id`       INT          NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `username` VARCHAR(30)  NOT NULL,
  `email`    VARCHAR(50)  NOT NULL,
  `password` VARCHAR(128) NOT NULL,
  `salt`     VARCHAR(128) NOT NULL)
  ENGINE = InnoDB
  DEFAULT CHARSET = utf8;

CREATE TABLE `login_attempts` (
  `user_id` INT(11)     NOT NULL,
  `time`    VARCHAR(30) NOT NULL)
  ENGINE = InnoDB
  DEFAULT CHARSET = utf8;

CREATE TABLE `strings` (
  `id`         INT(11)     NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `identifier` VARCHAR(30) NOT NULL,
  `language`   VARCHAR(5)  NOT NULL,
  `text`       VARCHAR(30) NOT NULL)
  ENGINE = InnoDB
  DEFAULT CHARSET = utf8;

CREATE TABLE `bots` (
  `id`         INT(11)                                            NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `account_id` INT(11)                                            NOT NULL,
  `name`       VARCHAR(30)                                        NOT NULL,
  `add_time`   DATETIME                                           NOT NULL,
  `code`       TEXT                                               NOT NULL,
  `code_lang`  ENUM('cpp', 'java', 'c#')                          NOT NULL,
  `state`      ENUM('processing', 'ok', 'runtime', 'compilation') NOT NULL
)
  ENGINE = InnoDB
  DEFAULT CHARSET = utf8;