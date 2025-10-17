# Required for SageMath
from sage.all import *

def orientation(p, q, r):
    """Return >0 if counter-clockwise, <0 if clockwise, 0 if collinear"""
    return (q[1] - p[1])*(r[0] - q[0]) - (q[0] - p[0])*(r[1] - q[1])

def gift_wrapping(points, visualize=False):
    """
    Compute convex hull using Gift Wrapping (Jarvis March).
    points: list of tuples [(x1,y1), (x2,y2), ...]
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
            # Step-by-step visualization
            show(
                sum([line([points[hull[i]], points[hull[i+1]]], color='blue') for i in range(len(hull)-1)], Graphics()) +
                line([points[p], points[q]], color='red') +
                sum([point(pt, color='black') for pt in points], Graphics())
            )
            pause(0.5)  # pause to visualize

        p = q
        if p == l:
            break

    return hull

# Example usage
import random
points = [(random.randint(0,50), random.randint(0,50)) for _ in range(15)]
hull = gift_wrapping(points, visualize=True)
print("Convex Hull Points:", hull)
