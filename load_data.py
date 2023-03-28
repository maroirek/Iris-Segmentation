import os 
from skimage import io
import torch 
import pandas as pd
from torch.utils.data import Dataset
from PIL import Image

class load_data(Dataset):
    def __init__(self, csv_file, transformer = None):
        self.csv = pd.read_csv(csv_file)   # the csv file must contain paths to each image with its label
        self.trans = transformer
        
    def __len__ (self):
        return int(len(self.csv)) 
    
    def __getitem__ (self, index):
        
        im_dir = self.csv.iloc[index, 0]
        img = Image.open(im_dir)
        img= img.convert("P", palette=Image.ADAPTIVE)
        
        label = torch.tensor(int(self.csv.iloc[index, 1]))
        
        if self.trans :
            img = self.trans(img)
    
        return ( img , label)
    
    