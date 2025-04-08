#!/usr/bin/env python3

import sys, json

log = open("/tmp/miv_plugin.log", "a")

def send(msg):
    sys.stdout.write(json.dumps(msg) + "\n")
    sys.stdout.flush()

def log_msg(text):
    log.write(text + "\n")
    log.flush()

for line in sys.stdin:
    try:
        req = json.loads(line)
        method = req.get("method")
        params = req.get("params", [])
        msg_id = req.get("id")

        if method == "miv.log":
            log_msg(f"[PLUGIN] Log: {params[0]}")
            send({"jsonrpc": "2.0", "result": "ok", "id": msg_id})

        elif method == "miv.ping":
            send({"jsonrpc": "2.0", "result": "pong", "id": msg_id})

        else:
            send({"jsonrpc": "2.0", "error": f"Unknown method {method}", "id": msg_id})

    except Exception as e:
        log_msg(f"[PLUGIN] Error: {e}")