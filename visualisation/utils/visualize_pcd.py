import k3d
import pandas as pd
import numpy as np
from ipywidgets import IntSlider, interact

def visualize_pointcloud(point_cloud, point_size = 1, flip_axes = False, name = 'Universe'):
    plot = k3d.plot(name = name, grid_visible = False, grid = (-0.55, -0.55, -0.55, 0.55, 0.55, 0.55))
    if flip_axes:
        point_cloud[:, 2] = point_cloud[:, 2] * -1
        point_cloud[:, [0, 1, 2]] = point_cloud[:, [0, 2, 1]]
    plt_points = k3d.points(positions=point_cloud.astype(np.float32), point_size = point_size, color = 0xd0d0d0)
    plot += plt_points
    plt_points.shader = '3d'
    plot.display()

def visualize_sim_frame(csv_file, frame_idx):
    vis_df = pd.read_csv(csv_file)
    frame = vis_df[vis_df["step"] == frame_idx] 
    points = frame[["x", "y", "z"]].to_numpy()
    visualize_pointcloud(points)

