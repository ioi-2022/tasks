# Coding Python is easier than LaTeX, so we generate the LaTeX code using Python
# instead. :)

def main():
  S = ["zero", "one", "two", "three", "four", "five", "six"]
  H = [10, 20, 60, 40, 50, 30, 70]
  leased = set({1, 3, 5})

  for i in range(len(H)):
    s = S[i]
    h = H[i] // 10
    print(f"\\tkzDefPoint({i + 0.2}, {0}){{{s}BL}}")
    print(f"\\tkzDefPoint({i + 0.4}, {h}){{{s}TL}}")
    print(f"\\tkzDefPoint({i + 0.8}, {0}){{{s}BR}}")
    print(f"\\tkzDefPoint({i + 0.6}, {h}){{{s}TR}}")

  for i in range(len(H)):
    s = S[i]
    t = "fill=black!50" if i in leased else ""
    print(f"\\tkzDrawPolygon[{t}]({s}BL, {s}BR, {s}TR, {s}TL)")

main()
