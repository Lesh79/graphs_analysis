import os
import gzip
import urllib.request

def download():
    FOLDER_NAME = 'graphs'

    GRAPHS = {
        'facebook_combined.txt': 'http://snap.stanford.edu/data/facebook_combined.txt.gz',
        'email-Enron.txt': 'https://snap.stanford.edu/data/email-Enron.txt.gz',
        'com-amazon.ungraph.txt': 'https://snap.stanford.edu/data/bigdata/communities/com-amazon.ungraph.txt.gz',
        'com-youtube.ungraph.txt': 'https://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz',
        'com-orkut.ungraph.txt': 'https://snap.stanford.edu/data/bigdata/communities/com-orkut.ungraph.txt.gz',
        'com-lj.ungraph.txt': 'https://snap.stanford.edu/data/bigdata/communities/com-lj.ungraph.txt.gz',


    }

    os.makedirs(FOLDER_NAME, exist_ok=True)

    for filename, url in GRAPHS.items():
        output_path = os.path.join(FOLDER_NAME, filename)

        if not os.path.exists(output_path):
            print(f"Downloading {filename}...")
            gz_path = os.path.join(FOLDER_NAME, f"{filename}.gz")

            urllib.request.urlretrieve(url, gz_path)

            with gzip.open(gz_path, 'rb') as f_in:
                with open(output_path, 'wb') as f_out:
                    f_out.write(f_in.read())

            os.remove(gz_path)
            print(f"Downloaded and extracted {filename}")
        else:
            print(f"{filename} already exists")

if (__name__ == "__main__"):
    download()