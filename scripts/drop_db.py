# USE CASE EXAMPLE:
# python drop_db.py config.ini

import sys
import os
import configparser

def exec_sql(auth, command):
  os.system(auth + " -c \"" + command + "\"")

if len(sys.argv) != 2:
  print("You have to pass the path of the config file to args:")
  print("python drop_db.py <path_to_config>")
  sys.exit()

config = configparser.ConfigParser()
config.read(sys.argv[1])

username = config["DbConfig"]["username"]
host = config["DbConfig"]["host"]
port = config["DbConfig"]["port"]

auth = "psql -U " + username + " -h " + host + " -p " + port

new_user = config["DbConfig"]["new_user"]
new_db = config["DbConfig"]["new_db"]
guest = config["DbConfig"]["guest_user"]

cmd_drop_db = "DROP DATABASE IF EXISTS " + new_db + ";"
cmd_drop_user = "DROP USER IF EXISTS " + new_user + ";"
cmd_drop_owned = "DROP OWNED BY " + guest + ";"
cmd_drop_guest = "DROP USER IF EXISTS " + guest + ";"

exec_sql(auth, cmd_drop_db)
exec_sql(auth, cmd_drop_user)
exec_sql(auth, cmd_drop_owned)
exec_sql(auth, cmd_drop_guest)