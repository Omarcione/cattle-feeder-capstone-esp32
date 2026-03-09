import pymysql
import os
import logging
import sys
import json
import datetime

username = os.environ['username']
password = os.environ['password']
endpoint = os.environ['endpoint']
dbName = os.environ['dbName']

logger = logging.getLogger()
logger.setLevel(logging.INFO)

try:
    conn = pymysql.connect(host=endpoint, user=username, password=password, database=dbName, connect_timeout=5)
    print("Success")
except pymysql.MySQLError as e:
    logger.error("ERROR: Could not connect to MySQL instance.")
    logger.error(e)
    sys.exit(1)

logger.info("SUCCESS: Connection to RDS for MySQL instance succeeded")

def lambda_handler(event, context):
    """
    This function creates a new RDS database table and writes records to it
    """
    data = event
    deviceID = data.get('deviceID')
    cowID = data.get('cowID')
    timeStr = data.get('time')
    format = "%Y-%m-%d %H:%M:%S"
    time = datetime.datetime.strptime(timeStr, format)
    weight = data.get('weight')
    co2 = data.get('co2')
    ch4 = data.get('ch4')

    with conn.cursor() as cur:

        if ((cowID == 0) and (weight == 0) and (co2 == 0) and (ch4 == 0)):
            sqlHeartbeat = "INSERT INTO heartbeat (time) VALUES (%s)"
            cur.execute(sqlHeartbeat, (timeStr,))
        else:
            # Add cowID to cow if it does not exist yet
            sqlCow = "INSERT IGNORE INTO cow (cowID) VALUES (%s)"
            cur.execute(sqlCow, (cowID,))   

            # Check if last session for this cow is within 5 min. 
            sqlCheck = """
                    SELECT sessionID, endTime FROM session 
                    WHERE cowID = %s 
                    ORDER BY endTime DESC LIMIT 1
                """
            cur.execute(sqlCheck, (cowID,))
            lastSession = cur.fetchone()
            sessionID = None

            if lastSession:
                lastSessionID, lastEndTime = lastSession
                if type(lastEndTime) is str:
                    lastEndTime = datetime.datetime.strptime(lastEndTime, format)
                # Calculate timeDiff between last session
                timeDiff = (time - lastEndTime).total_seconds()
                if timeDiff < 300:
                    # Within 5 mins: Use existing session
                    sessionID = lastSessionID
                    sqlUpdateSession = "UPDATE session SET endTime = %s WHERE sessionID = %s"
                    cur.execute(sqlUpdateSession, (timeStr, sessionID))
            
            if sessionID is None:
                sqlNewSession = "INSERT INTO session (startTime, endTime, cowID) VALUES (%s, %s, %s)"
                cur.execute(sqlNewSession, (timeStr, timeStr, cowID))
                sessionID = cur.lastrowid


            sqlData = "INSERT INTO data (time, weight, co2, ch4, sessionID) VALUES (%s, %s, %s, %s, %s)"
            cur.execute(sqlData, (timeStr, weight, co2, ch4, sessionID))    
    conn.commit()

    return 
