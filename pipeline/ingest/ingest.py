import sqlite3
import json
conn = sqlite3.connect('pipeline/db/packets.db')

cursor = conn.cursor()
cursor.execute("""
    CREATE TABLE IF NOT EXISTS packets (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp TEXT,
        src_ip TEXT,
        dst_ip TEXT,
        protocol INTEGER,
        src_prt INTEGER,
        dst_prt INTEGER
    )
""")
conn.commit()
# TODO: capture duplicates later

with open('capture/output/packets.jsonl', 'r') as f:
    for line in f:
        packet = json.loads(line)
        cursor.execute(""" INSERT INTO packets (timestamp, src_ip, dst_ip, protocol, src_port, dst_port) VALUES (?, ?, ?, ?, ?, ?)""", (packet['timestamp'], packet['src_ip'], packet['dst_ip'], packet['protocol'], packet.get('src_port'), packet.get('dst_port')))
        
    conn.commit()
    cursor.execute("SELECT COUNT(*) FROM packets")
    count = cursor.fetchone()[0]
    print(f"Inserted {count} rows into packets table")


cursor.execute("""
    CREATE TABLE IF NOT EXISTS alerts (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        packet_id INTEGER,
        rule_name   TEXT,
        message TEXT,
        severity TEXT,
        timestamp  TEXT     
    )
""")
conn.commit()