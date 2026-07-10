import sqlite3

conn = sqlite3.connect('pipeline/db/packets.db')
conn.row_factory = sqlite3.Row

cursor = conn.cursor()
cursor.execute("SELECT * FROM packets")
rows = cursor.fetchall()
for row in rows:
    print(row['id'], row['src_ip'], row['dst_ip'], row['protocol'], row['src_port'], row['dst_port'])

def detect_icmp_flood(cursor, conn, threshold=5):
    cursor.execute("""
        SELECT src_ip, COUNT(*) as count, id
        FROM packets
        WHERE protocol = 1
        GROUP BY src_ip
        HAVING count > ?
    """, (threshold,))
    
    results = cursor.fetchall()
    print(f"Found {len(results)} matches")
    insert_cursor = conn.cursor()
    for row in results:
        insert_cursor.execute("""
            INSERT INTO alerts (packet_id, rule_name, message, severity, timestamp)
            VALUES (?, ?, ?, ?, datetime('now'))
        """, (row['id'], 'icmp_flood', f"ICMP flood from {row['src_ip']} ({row['count']} packets)", 'medium'))


def detect_suspicious_port(cursor, conn):
    cursor.execute("""
        SELECT src_ip, dst_port, id
        FROM packets
        WHERE dst_port IN (4444, 1337, 31337)
    """)
    
    results = cursor.fetchall()
    print(f"Found {len(results)} matches")
    insert_cursor = conn.cursor()
    for row in results:
        insert_cursor.execute("""
            INSERT INTO alerts (packet_id, rule_name, message, severity, timestamp)
            VALUES (?, ?, ?, ?, datetime('now'))
        """, (row['id'], 'suspicious_port', f"Suspicious port activity from {row['src_ip']} to port {row['dst_port']} (packets)", 'high'))
    
detect_icmp_flood(cursor, conn)
detect_suspicious_port(cursor, conn)
conn.commit()
print("Detection complete.")