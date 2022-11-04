-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema 'ATTEND_DB'
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema 'ATTEND_DB'
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `ATTEND_DB` DEFAULT CHARACTER SET utf8 ;
USE `ATTEND_DB` ;

-- -----------------------------------------------------
-- Table `ATTEND_DB`.`USER_ATTEND_SETTING`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`USER_ATTEND_SETTING` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`USER_ATTEND_SETTING` (
  `user_id` VARCHAR(45) NOT NULL COMMENT '사용자 id',
  `device_id` VARCHAR(45) NULL COMMENT '사용자 기기 id',
  `setted_attend_time` INT NULL COMMENT '설정된 사용자의 출근 시간',
  `setted_off_time` INT NULL COMMENT '설정된 사용자의 퇴근 시간',
  `must_work_time` INT NULL COMMENT '의무 근로 시간',
  `break_time` INT NULL COMMENT '휴식시간(ex-점심시간)',
  `vacation_year` INT NULL COMMENT '남은 연차 개수',
  `vacation_month` INT NULL COMMENT '남은 월차 개수',
  PRIMARY KEY (`user_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`USER_INFO`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`USER_INFO` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`USER_INFO` (
  `user_id` VARCHAR(10) NOT NULL COMMENT '사용자 id',
  `user_nm` VARCHAR(45) NOT NULL COMMENT '사용자 이름',
  `company_id` VARCHAR(10) NOT NULL COMMENT '사번',
  `department_id` VARCHAR(10) NOT NULL COMMENT '부서 id',
  `passwd` VARCHAR(45) NOT NULL COMMENT '비밀번호',
  `user_type` INT NOT NULL COMMENT '사용자정보(일반-0, 부서관리자-1, 전체관리자-2)',
  `user_image_path` VARCHAR(255) NULL COMMENT '사용자 사진 경로',
  PRIMARY KEY (`user_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`DEPARTMENT_INFO`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`DEPARTMENT_INFO` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`DEPARTMENT_INFO` (
  `department_id` VARCHAR(10) NOT NULL COMMENT '부서 id',
  `department_nm` VARCHAR(45) NOT NULL COMMENT '부서 이름',
  `upper_department_id` VARCHAR(10) NULL COMMENT '상위 부서 id',
  PRIMARY KEY (`department_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`USER_STATE`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`USER_STATE` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`USER_STATE` (
  `num` INT NOT NULL COMMENT '자동 증가',
  `user_id` VARCHAR(10) NOT NULL COMMENT '사용자 id',
  `attend_state` INT NULL COMMENT '출근 상태(출근-0, 퇴근-1)',
  `attend_time` DATETIME NULL COMMENT '출근 시각',
  `off_time` DATETIME NULL COMMENT '퇴근 시각',
  `attend_place` VARCHAR(255) NULL COMMENT '출근 위치',
  `off_place` VARCHAR(255) NULL COMMENT '퇴근 위치',
  `attend_day` VARCHAR(10) NULL COMMENT '출근 요일',
  `attend_date` DATE NULL COMMENT '출근 날짜',
  `work_time_per_day` INT NULL COMMENT '하루 근무 시간',
  `over_time` INT NULL COMMENT '연장 근로 시간',
  `normal` INT NULL COMMENT '정상근무-1,아닐경우-0',
  `trip` INT NULL COMMENT '출장-1,아닐경우-0',
  `vacation` INT NULL COMMENT '휴가-1,아닐경우-0',
  `outside` INT NULL COMMENT '외근-1,아닐경우-0',
  `education` INT NULL COMMENT '교육-1,아닐경우-0',
  `weekend` INT NULL COMMENT '주말-1,아닐경우-0',
  `holiday` INT NULL COMMENT '공휴일-1,아닐경우-0',
  `sick` INT NULL COMMENT '병가-1,아닐경우-0',
  `endtype` VARCHAR(45) NULL COMMENT 'etc',
  PRIMARY KEY (`num`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`VACATION`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`VACATION` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`VACATION` (
  `num` INT NOT NULL COMMENT '자동 증가',
  `user_id` VARCHAR(10) NOT NULL COMMENT '사용자 id',
  `vacation_type` INT NOT NULL COMMENT '연차-0,월차-1,병가-2',
  `vacation_start_date` DATE NOT NULL COMMENT '휴가 시작 일자',
  `vacation_end_date` DATE NOT NULL COMMENT '휴가 끝나는 날짜',
  `vacation_period` INT NULL COMMENT '휴가 기간',
  `vacation_reason` VARCHAR(255) NULL COMMENT '휴가 사유',
  `vacation_permit` INT NULL COMMENT '신청-0,허용-1,반려-2',
  PRIMARY KEY (`num`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`BUSINESS_TRIP`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`BUSINESS_TRIP` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`BUSINESS_TRIP` (
  `num` INT NOT NULL COMMENT '자동 증가',
  `user_id` VARCHAR(10) NOT NULL COMMENT '사용자 id',
  `business_start_date` DATE NOT NULL COMMENT '출장 시작 날짜',
  `business_end_date` DATE NOT NULL COMMENT '출장 끝나는 날짜',
  `business_period` INT NULL COMMENT '휴가 기간',
  `business_reason` VARCHAR(255) NULL COMMENT '출장 사유',
  `business_permit` INT NULL COMMENT '신청-0,허용-1,반려-2',
  `business_place` VARCHAR(255) NULL COMMENT '출장 장소',
  PRIMARY KEY (`num`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`OUTSIDE`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`OUTSIDE` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`OUTSIDE` (
  `num` INT NOT NULL COMMENT '자동 증가',
  `user_id` VARCHAR(10) NOT NULL COMMENT '사용자 id',
  `outside_type` INT NOT NULL COMMENT '외근-0,교육-1',
  `outside_start_time` DATETIME NOT NULL COMMENT '외근/교육 시작 시각',
  `outside_end_time` DATETIME NOT NULL COMMENT '외근/교육 끝나는 시각',
  `outside_place` VARCHAR(255) NULL COMMENT '외근/교육 장소',
  `outside_reason` VARCHAR(255) NULL COMMENT '외근/교육 사유',
  `outside_permit` INT NULL COMMENT '신청-0,허용-1,반려-2',
  PRIMARY KEY (`num`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `ATTEND_DB`.`OVER_WORK`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `ATTEND_DB`.`OVER_WORK` ;

CREATE TABLE IF NOT EXISTS `ATTEND_DB`.`OVER_WORK` (
  `num` INT NOT NULL COMMENT '자동 증가',
  `user_id` VARCHAR(10) NOT NULL COMMENT '사용자 id',
  `over_type` INT NOT NULL COMMENT '연장근무-0,주말-1,공휴일-2',
  `over_start_time` DATETIME NULL COMMENT '시작 시각',
  `over_end_time` DATETIME NULL COMMENT '끝나는 시각',
  `over_reason` VARCHAR(255) NULL COMMENT '근무 사유',
  `over_permit` INT NULL COMMENT '0-신청,1-허용,2-반려',
  PRIMARY KEY (`num`))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
