# USE CASE EXAMPLE:
# python create_db.py config.ini

import sys
import os
import configparser


def exec_sql(auth, command):
  os.system(auth + " -c \"" + command + "\"")

def exec_sql_for_db(auth, db, command):
  os.system(auth + " -d " + db + " -c \"" + command + "\"")

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

guest_user = config["DbConfig"]["guest_user"]
guest_pass = config["DbConfig"]["guest_pass"]


cmd_create_user = "CREATE USER " + new_user + " WITH PASSWORD '" + new_pass + "';"
cmd_create_guest = "CREATE USER " + guest_user + " WITH PASSWORD '" + guest_pass + "';"
cmd_create_db = "CREATE DATABASE " + new_db + ";"

cmd_give_grant = "GRANT ALL PRIVILEGES ON DATABASE " + new_db + " TO " + new_user + ";"

cmd_give_grant_usage = "GRANT USAGE ON SCHEMA public TO " + guest_user + ";"
cmd_give_grant_select = "GRANT SELECT ON ALL TABLES IN SCHEMA public TO " + guest_user + ";"
cmd_give_grant_alter = "ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT ON TABLES TO " + guest_user + ";"

cmd_create_host = "sql/host.sql"
cmd_create_asset = "sql/asset.sql"


auth = make_auth(username, host, port)

exec_sql(auth, cmd_create_user)
exec_sql(auth, cmd_create_guest)
exec_sql(auth, cmd_create_db)
exec_sql(auth, cmd_give_grant)


auth = make_auth(new_user, host, port)

exec_sql_for_db(auth, new_db, cmd_give_grant_usage)
exec_sql_for_db(auth, new_db, cmd_give_grant_select)
exec_sql_for_db(auth, new_db, cmd_give_grant_alter)

exec_file_for_db(auth, new_db, cmd_create_host)
exec_file_for_db(auth, new_db, cmd_create_asset)