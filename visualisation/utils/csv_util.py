import pandas as pd
import numpy as np

def csv_to_pcd(csv_file):
    sim_df = pd.read_csv(csv_file)
    flat_pcd = sim_df[["x", "y", "z"]].to_numpy()
    np_video_tensor = np.reshape(flat_pcd, (max(sim_df["step"])+1, max(sim_df["body"])+1, 3))
    return np_video_tensor


if __name__ == "__main__":
    csv_to_pcd("./results/barnes-hut.csv")