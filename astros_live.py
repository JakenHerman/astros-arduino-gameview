import requests
import time
import serial
import json
import argparse
import sys

# ================== CONFIG ==================
TEAM_ID = 117

def main():
    parser = argparse.ArgumentParser(description="Astros Live Scoreboard Updater")
    parser.add_argument(
        "-p", "--port",
        help="Arduino serial port"
    )
    parser.add_argument(
        "--baud",
        type=int,
        default=9600,
        help="Baud rate (default: 9600)"
    )
    args = parser.parse_args()

    # Connect to Arduino
    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        print(f"✅ Connected to Arduino on {args.port}")
    except Exception as e:
        ser = None
        print(f"⚠️ Could not open {args.port} — running in debug mode only")
        print(f"   Error: {e}")

    def get_astros_game_data():
        try:
            today = time.strftime("%Y-%m-%d")
            sched_url = f"https://statsapi.mlb.com/api/v1/schedule?sportId=1&teamId={TEAM_ID}&date={today}"
            sched = requests.get(sched_url, timeout=10).json()

            games = sched.get("dates", [{}])[0].get("games", [])
            if not games:
                print("No Astros game today.")
                return None

            game = games[0]
            game_pk = game["gamePk"]
            status = game.get("status", {}).get("detailedState", "Unknown")

            print(f"Game found — Status: {status} | gamePk: {game_pk}")

            live_url = f"https://statsapi.mlb.com/api/v1/game/{game_pk}/feed/live"
            data = requests.get(live_url, timeout=10).json()

            live_data = data.get("liveData", {})
            linescore = live_data.get("linescore", {})

            packet = {
                "home_runs": 0,
                "away_runs": 0,
                "inning": 1,
                "top": True,
                "balls": 0,
                "strikes": 0,
                "outs": 0,
                "on1b": False,
                "on2b": False,
                "on3b": False,
                "status": status
            }

            if linescore:
                home = linescore.get("teams", {}).get("home", {})
                away = linescore.get("teams", {}).get("away", {})

                packet.update({
                    "home_runs": home.get("runs", 0),
                    "away_runs": away.get("runs", 0),
                    "inning": linescore.get("currentInning", 1),
                    "top": linescore.get("isTopInning", True),
                    "balls": linescore.get("balls", 0),
                    "strikes": linescore.get("strikes", 0),
                    "outs": linescore.get("outs", 0),
                })

            return packet

        except Exception as e:
            print(f"Error fetching MLB data: {e}")
            return None

    print("🚀 Astros Live Scoreboard Updater started")
    print(f"Using port: {args.port}")
    print("Press Ctrl+C to stop.\n")

    while True:
        data = get_astros_game_data()

        if data:
            print(f"→ {data['status']} | Inning {data['inning']} {'Top' if data['top'] else 'Bot'} | "
                  f"B:{data['balls']} S:{data['strikes']} O:{data['outs']} | "
                  f"HOU {data['home_runs']} - {data['away_runs']}")

            if ser:
                try:
                    ser.write((json.dumps(data) + "\n").encode())
                    ser.flush()
                except Exception as e:
                    print(f"Serial write error: {e}")
        else:
            print("No data available right now.")

        time.sleep(15)


if __name__ == "__main__":
    main()
