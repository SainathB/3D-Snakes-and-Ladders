import re
import bs4
import requests
import subprocess
from sys import argv

# num = argv[1]
for x in xrange(int(argv[1]),100):
	num = x
	gurl = "https://www.google.co.in/search?q=%s+filetype:jpg&tbm=isch"
	html = requests.get(gurl % num).text
	soup = bs4.BeautifulSoup(html)
	link = soup.select(".images_table img")[0]
	iurl = link.attrs["src"]
	print iurl
	# iurl = re.search(r".*imgurl\=(.*)\&", link).groups()[0]
	subprocess.call(["wget", "-O", "number%s.jpg" % num, iurl])

# baseurl = "http://www.kidsmathgamesonline.com/images/pictures/numbers120/number%s.jpg"
# for x in xrange(40, 100):
# 	subprocess.call(["wget", baseurl % x])
