# USE CASE EXAMPLE:
# python create_db.py config.ini

import sys
import os
import configparser


def exec_sql(auth, command):
  os.system(auth + " -c \"" + command + "\"")

def exec_file_for_db(auth, db, file):
  os.system(auth + " -d " + db + " -f " + file)

def make_auth(username, host, port):
  return "psql -U " + username + " -h " + host + " -p " + port


if len(sys.argv) != 2:
  print("You have to pass the path of the config file to args:")
  print("python create_db.py <path_to_config>")
  sys.exit()


config = configparser.ConfigParser()
config.read(sys.argv[1])


username = config["DbConfig"]["username"]
host = config["DbConfig"]["host"]
port = config["DbConfig"]["port"]

new_user = config["DbConfig"]["new_user"]
new_pass = config["DbConfig"]["new_pass"]
new_db = config["DbConfig"]["new_db"]


cmd_create_user = "CREATE USER " + new_user + " with PASSWORD '" + new_pass + "';"
cmd_create_db = "CREATE DATABASE " + new_db + ";"
cmd_give_grant = "GRANT ALL PRIVILEGES ON DATABASE " + new_db + " TO " + new_user + ";"

cmd_create_status = "sql/status.sql"
cmd_create_service = "sql/service.sql"
cmd_create_port = "sql/port.sql"
cmd_create_host = "sql/host.sql"
cmd_create_host_port = "sql/host_port.sql"
cmd_create_scanresult =  "sql/scanresult.sql"
cmd_create_scanresult_host = "sql/scanresult_host.sql"


auth = make_auth(username, host, port)

exec_sql(auth, cmd_create_user)
exec_sql(auth, cmd_create_db)
exec_sql(auth, cmd_give_grant)


auth = make_auth(new_user, host, port)

exec_file_for_db(auth, new_db, cmd_create_status)
exec_file_for_db(auth, new_db, cmd_create_service)
exec_file_for_db(auth, new_db, cmd_create_port)
exec_file_for_db(auth, new_db, cmd_create_host)
exec_file_for_db(auth, new_db, cmd_create_host_port)
exec_file_for_db(auth, new_db, cmd_create_scanresult)