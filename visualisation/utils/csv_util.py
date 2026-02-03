import pandas as pd
import numpy as np

def csv_to_pcd(csv_file):
    sim_df = pd.read_csv(csv_file)
    flat_pcd = sim_df[["x", "y", "z"]].to_numpy()
    np_video_tensor = np.reshape(flat_pcd, (max(sim_df["step"]) + 1, max(sim_df["body"]) + 1, 3))
    return np_video_tensor

def bin_to_pcd(bin_file):
    dtype = np.dtype([
    ("body_id", "<u4"),   # uint32
    ("step", "<u4"),     
    ("x", "<f8"),   # float64
    ("y", "<f8"),
    ("z", "<f8"),
    ])
    arr = np.fromfile(bin_file, dtype=dtype)
    num_steps = arr["step"].max() + 1
    pos = np.column_stack((arr["x"], arr["y"], arr["z"]))
    assert pos.shape[0] % num_steps == 0
    pos = pos.reshape((num_steps,-1, 3))
    return pos

if __name__ == "__main__":
    bin_to_pcd("./results/bh.bin")
    csv_to_pcd("./results/bh.csv")