import sqlite3

conn = sqlite3.connect('pipeline/db/packets.db')
conn.row_factory = sqlite3.Row

cursor = conn.cursor()
cursor.execute("SELECT * FROM packets")
rows = cursor.fetchall()
for row in rows:
    print(row['id'], row['src_ip'], row['dst_ip'], row['protocol'], row['src_port'], row['dst_port'])
