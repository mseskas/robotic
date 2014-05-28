'''
Lucas-Kanade tracker
====================

Lucas-Kanade sparse optical flow demo. Uses goodFeaturesToTrack
for track initialization and back-tracking for match verification
between frames.

Usage
-----
lk_track.py [<video_source>]


Keys
----
ESC - exit
'''

import numpy as np
import cv2
import video
from common import anorm2, draw_str
from time import clock
import pdb

lk_params = dict( winSize  = (15, 15), 
                  maxLevel = 2, 
                  criteria = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))    

feature_params = dict( maxCorners = 50, # 500, 
                       qualityLevel = 0.01, # 0.3,
                       minDistance = 10, # 7,
                       blockSize = 5 ) # 7 )

class App:
    def __init__(self, video_src):
        self.track_len = 10
        self.detect_interval = 5
        self.tracks = []
        self.cam = video.create_capture(video_src)
        self.frame_idx = 0

    def run(self):
        while True:
            ret, frame = self.cam.read()
            frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            vis = frame.copy()

            if len(self.tracks) > 0:
                img0, img1 = self.prev_gray, frame_gray
                p0 = np.float32([tr[-1] for tr in self.tracks]).reshape(-1, 1, 2)
                p1, st, err = cv2.calcOpticalFlowPyrLK(img0, img1, p0, None, **lk_params)
                p0r, st, err = cv2.calcOpticalFlowPyrLK(img1, img0, p1, None, **lk_params)
                d = abs(p0-p0r).reshape(-1, 2).max(-1)
                good = d < 1
                new_tracks = []
                for tr, (x, y), good_flag in zip(self.tracks, p1.reshape(-1, 2), good):
                    if not good_flag:
                        continue
                    tr.append((x, y))
                    if len(tr) > 2 : # self.track_len:
                        del tr[1]
                    new_tracks.append(tr)
                    cv2.circle(vis, (x, y), 2, (0, 255, 0), -1)
                self.tracks = new_tracks
                cv2.polylines(vis, [np.int32(tr) for tr in self.tracks], False, (0, 255, 0))
                draw_str(vis, (20, 20), 'track count: %d' % len(self.tracks))

            # if self.frame_idx % self.detect_interval == 0:
            if len(self.tracks) < 400:
                mask = np.zeros_like(frame_gray)
                mask[:] = 255
                for x, y in [np.int32(tr[-1]) for tr in self.tracks]:
                    cv2.circle(mask, (x, y), 5, 0, -1)
                p = cv2.goodFeaturesToTrack(frame_gray, mask = mask, **feature_params)
                if p is not None:
                    for x, y in np.float32(p).reshape(-1, 2):
                        self.tracks.append([(x, y)])


            self.frame_idx += 1
            self.prev_gray = frame_gray
            cv2.imshow('lk_track', vis)
            
            self.get_rotation()
            
            
            ch = 0xFF & cv2.waitKey(1)
            if ch == 27:
                break
    
    def get_rotation(self):
        # print self.tracks
        # self.tracks[0][0][0] - self.tracks[0][1][0]
        # self.tracks[0][0][1] - self.tracks[0][1][1]
        if self.tracks is not None:
            if len(self.tracks[0]) < 2:
                return None
        else:
            return None
        
        x_dif = 0.0
        y_dif = 0.0
        curr_line = None
        try:
            for line in self.tracks:
                if len(line) == 2:
                    curr_line = line
                    x_dif += line[1][0] - line[0][0]
                    y_dif += line[1][1] - line[0][1]
        except Exception:
            pdb.set_trace()


        x_dif /= len(self.tracks)
        y_dif /= len(self.tracks)
        print "( " + str(x_dif) + ", " + str(y_dif) + ")\n" 

def main():
    import sys
    try: video_src = sys.argv[1]
    except: video_src = 0

    print __doc__
    App(video_src).run()
    cv2.destroyAllWindows() 			

if __name__ == '__main__':
    main()
