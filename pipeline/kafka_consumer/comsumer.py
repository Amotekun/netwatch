import json
import sqlite3
from confluent_kafka import Consumer, KafkaException

sqlite_db_path = "pipeline/db/packets.db"

conn = sqlite3.connect(sqlite_db_path)
cursor = conn.cursor()

conf = {
    "bootstrap.servers": "localhost:9092",
    "group.id": "netwatch-ingest",
    "auto.offset.reset": "earliest",
}
consumer = Consumer(conf)

consumer.subscribe(["packets"])

try:
    while True:
        msg = consumer.poll(timeout=1.0)

        if msg is None:
            continue

        if msg.error():
            raise KafkaException(msg.error())

        raw = msg.value()
        if raw is None:
            print("Received empty message")
            continue

        packets = json.loads(raw.decode("utf-8"))

        cursor.execute(
            """
            INSERT INTO packets (timestamp, src_ip, dst_ip, protocol, src_port, dst_port) 
            VALUES (?, ?, ?, ?, ?, ?)
            """,
            (
                packets["timestamp"],
                packets["src_ip"],
                packets["dst_ip"],
                packets["protocol"],
                packets.get("src_port"),
                packets.get("dst_port"),
            ),
        )

        conn.commit()
        print(f"Inserted: {packets['src_ip']} -> {packets['dst_ip']}")

except KeyboardInterrupt:
    print("Consumer Interrupted by user")

finally:
    consumer.close()
    conn.close()
