import matplotlib.pyplot as plt
import random
import time

def orientation(p, q, r):
    """Return >0 if counter-clockwise, <0 if clockwise, 0 if collinear"""
    return (q[1] - p[1])*(r[0] - q[0]) - (q[0] - p[0])*(r[1] - q[1])

def gift_wrapping(points, visualize=False, pause_time=0.5):
    """
    Compute convex hull using Gift Wrapping (Jarvis March)
    points: list of (x,y) tuples
    visualize: if True, show step-by-step plot
    """
    n = len(points)
    if n < 3:
        return points

    hull = []

    # Step 1: Find leftmost point
    l = min(range(n), key=lambda i: points[i][0])
    p = l

    while True:
        hull.append(points[p])
        q = (p + 1) % n
        for i in range(n):
            if orientation(points[p], points[i], points[q]) < 0:
                q = i

        if visualize:
            plt.clf()
            # Plot all points
            xs, ys = zip(*points)
            plt.scatter(xs, ys, color='black')
            # Plot hull edges so far
            if len(hull) > 1:
                hx, hy = zip(*hull)
                plt.plot(hx, hy, color='blue', linewidth=2)
            # Plot current candidate edge in red
            plt.plot([points[p][0], points[q][0]], [points[p][1], points[q][1]], color='red', linestyle='--')
            plt.scatter(*points[p], color='green', s=100)  # current point
            plt.title("Gift Wrapping / Convex Hull Step")
            plt.pause(pause_time)

        p = q
        if p == l:
            break

    # Close the hull for plotting
    hull.append(hull[0])

    if visualize:
        plt.clf()
        xs, ys = zip(*points)
        plt.scatter(xs, ys, color='black')
        hx, hy = zip(*hull)
        plt.plot(hx, hy, color='blue', linewidth=2)
        plt.title("Final Convex Hull")
        plt.show()

    return hull[:-1]  # remove duplicate last point

# ----------------------------
# Example usage
# ----------------------------
if __name__ == "__main__":
    # Generate random points
    points = [(random.randint(0, 50), random.randint(0, 50)) for _ in range(20)]
    
    print("Points:", points)
    
    hull = gift_wrapping(points, visualize=True, pause_time=0.5)
    print("Convex Hull Points:", hull)
