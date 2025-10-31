import os
import random
import re
import sys

DAMPING = 0.85
SAMPLES = 10000


def main():
    if len(sys.argv) != 2:
        sys.exit("Usage: python pagerank.py corpus")
    corpus = crawl(sys.argv[1])
    ranks = sample_pagerank(corpus, DAMPING, SAMPLES)
    print(f"PageRank Results from Sampling (n = {SAMPLES})")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")
    ranks = iterate_pagerank(corpus, DAMPING)
    print(f"PageRank Results from Iteration")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")


def crawl(directory):
    """
    Parse a directory of HTML pages and check for links to other pages.
    Return a dictionary where each key is a page, and values are
    a list of all other pages in the corpus that are linked to by the page.
    """
    pages = dict()

    # Extract all links from HTML files
    for filename in os.listdir(directory):
        if not filename.endswith(".html"):
            continue
        with open(os.path.join(directory, filename)) as f:
            contents = f.read()
            links = re.findall(r"<a\s+(?:[^>]*?)href=\"([^\"]*)\"", contents)
            pages[filename] = set(links) - {filename}

    # Only include links to other pages in the corpus
    for filename in pages:
        pages[filename] = set(
            link for link in pages[filename]
            if link in pages
        )

    return pages


def transition_model(corpus, page, damping_factor):
    """
    Return a probability distribution over which page to visit next,
    given a current page.

    With probability `damping_factor`, choose a link at random
    linked to by `page`. With probability `1 - damping_factor`, choose
    a link at random chosen from all pages in the corpus.
    """

    probabilities = dict()

    numPages = len(corpus)

    # not the best practice, since im modifying the corpus
    #if not corpus[page]:
    #    corpus[page] = set(corpus.keys())
    links = corpus[page] if corpus[page] else set(corpus.keys())

    for primary_page in corpus:
        prob = (1-damping_factor)/(numPages)
        if primary_page in links:
            prob += damping_factor / len(links)
        probabilities[primary_page] = prob

    return probabilities


def sample_pagerank(corpus, damping_factor, n):
    """
    Return PageRank values for each page by sampling `n` pages
    according to transition model, starting with a page at random.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """
    pageRank = dict()
    for page in corpus:
        pageRank[page] = 0

    prev_page = random.choice(list(corpus.keys()))

    for i in range(n):
        #page = corpus[prev_page]
        probs = transition_model(corpus, prev_page, damping_factor)
        # Option 1: represent the probabilities in the transition model as a 
        # cumulative distribution and generate a random float to select the 
        # next page
        #cumulative = 0
        #choice = random.random()
        #for new_page in probs:
        #    cumulative = cumulative + probs[new_page]
        #    if cumulative >= choice:
        #        next_page = new_page
        #        break
        # Option 2: use the random.choices() to select it
        next_page = random.choices(list(probs.keys()), weights=probs.values(), k=1)[0]

        pageRank[next_page] += 1
        
        prev_page = next_page

    # produce the actual probability given number of times landed on / total number of samples
    for page in pageRank:
        pageRank[page] = pageRank[page] / n

    return pageRank

    



def iterate_pagerank(corpus, damping_factor):
    """
    Return PageRank values for each page by iteratively updating
    PageRank values until convergence.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """
    prevPageRank = dict()
    newPageRank = dict()
    # initialize each page to 1 / N
    n = len(corpus)
    if n <= 0:
        print("Corpus is empty")
        return prevPageRank
    
    for page in corpus:
        prevPageRank[page] = 1 / n

    while True:
        newPageRank = {}
        tol = 0
        for page in prevPageRank:
            prSum = 0
            for linkedPage in corpus:
                # dont forget, we covered the empty list in sample(initially with transition model function, but changed that a little), but we didn't account for that here
                if len(corpus[linkedPage]) == 0:
                    prSum += prevPageRank[linkedPage] / n
                elif page in corpus[linkedPage]:
                    prSum += prevPageRank[linkedPage]/len(corpus[linkedPage])
                    
            newPageRank[page] = ((1 - damping_factor) / n) + (damping_factor * prSum)
            tol = max(tol, abs(newPageRank[page] - prevPageRank[page]))

        if tol < 0.001:
            break
        prevPageRank.update(newPageRank)

    # normalize to ensure total is 1, also helps with floating point issues
    total = sum(prevPageRank.values())
    for p in prevPageRank:
        prevPageRank[p] /= total

    return prevPageRank
    


if __name__ == "__main__":
    main()
