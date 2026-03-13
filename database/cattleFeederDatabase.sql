CREATE TABLE cow (
	cowID BIGINT UNSIGNED PRIMARY KEY
);

CREATE TABLE session (
	sessionID int AUTO_INCREMENT PRIMARY KEY,
    startTime datetime NOT NULL,
    endTime datetime NOT NULL, 
    cowID BIGINT UNSIGNED NOT NULL,
    FOREIGN KEY (cowID) REFERENCES cow (cowID)
);


CREATE TABLE data (
	dataID int AUTO_INCREMENT PRIMARY KEY,
    time datetime NOT NULL,
    weight float NOT NULL,
    co2 float NOT NULL,
    ch4 float NOT NULL,
    sessionID int NOT NULL,
    FOREIGN KEY (sessionID) REFERENCES session (sessionID)
);

CREATE TABLE heartbeat (
	time datetime NOT NULL
);


CREATE OR REPLACE VIEW sessionData AS
SELECT 
    s.sessionID,
    s.cowID,
    DATE(s.startTime) AS sessionDate,
    ROUND(
        (SELECT weight FROM data d WHERE d.sessionID = s.sessionID ORDER BY d.time ASC LIMIT 1) - 
        (SELECT weight FROM data d WHERE d.sessionID = s.sessionID ORDER BY d.time DESC LIMIT 1), 
    1) AS sessionAmountEaten,

    (SELECT TIMESTAMPDIFF(MINUTE, MIN(time), MAX(time)) 
     FROM data d 
     WHERE d.sessionID = s.sessionID) AS sessionMinutes,
     
     (SELECT AVG(co2) 
     FROM data d 
     WHERE d.sessionID = s.sessionID) AS sessionAvgCO2
FROM session s;


CREATE OR REPLACE VIEW dailyData AS
SELECT 
    cowID,
    sessionDate,
    COUNT(sessionID) AS numSessions,
    ROUND(SUM(sessionAmountEaten), 3) AS totalDailyConsumption,
    SUM(sessionMinutes) AS totalDailyFeedingMinutes,
    AVG(sessionAvgCO2) AS dailyAvgCO2
FROM sessionData
GROUP BY cowID, sessionDate;
