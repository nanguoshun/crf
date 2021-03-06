namespace NLP {
  namespace CRF {
    class Feature;

    typedef std::vector<Feature *> FeaturePtrs;

    /**
     * Context object.
     * Stores the observed tagpair at position index in a sentence, along with
     * a vector of pointers to features active at that position given the
     * observations (e.g. words).
     */
    class Context {
      public:
        FeaturePtrs features;
        TagPairs klasses;
        size_t index;

        Context(void) : features(), klasses(), index(0) { }
        Context(TagPair kl, const size_t index=0)
          : features(), klasses(), index(index) {
            klasses.push_back(kl);
        }
        Context(Tag prev, Tag curr, const size_t index=0)
          : features(), klasses(), index(index) {
          klasses.push_back(TagPair(prev, curr));
        }

        bool klasses_match(TagPair &other) const {
          for (size_t i = 0; i < klasses.size(); ++i)
            if (klasses[i] == other)
              return true;
          return false;
        }

        bool klasses_match_or_none(TagPair &other) const {
          for (size_t i = 0; i < klasses.size(); ++i)
            if (klasses[i] == other || (other.prev == None::val && other.curr == klasses[i].curr))
              return true;
          return false;
        }
    };

    /**
     * Contexts object.
     *
     * Thin wrapper around a vector of Context objects. Represents a training
     * instance, where the i'th word in a sentence corresponds to the i'th
     * item in the contexts vector.
     */
    class Contexts {
      private:
        std::vector<Context> contexts;

      public:
        Contexts(void) : contexts() { }
        Contexts(const size_t size)
          : contexts(size) { }

        size_t size(void) const { return contexts.size(); }

        Context &operator[](const size_t index) {
          return contexts[index];
        }

        typedef std::vector<Context>::iterator iterator;
        typedef std::vector<Context>::const_iterator const_iterator;
        typedef std::vector<Context>::reverse_iterator reverse_iterator;
        typedef std::vector<Context>::const_reverse_iterator const_reverse_iterator;

        iterator begin(void) { return contexts.begin(); }
        const_iterator begin(void) const { return contexts.begin(); }
        iterator end(void) { return contexts.end(); }
        const_iterator end(void) const { return contexts.end(); }

        reverse_iterator rbegin(void) { return contexts.rbegin(); }
        const_reverse_iterator rbegin(void) const { return contexts.rbegin(); }
        reverse_iterator rend(void) { return contexts.rend(); }
        const_reverse_iterator rend(void) const { return contexts.rend(); }
    };

    typedef std::vector<Contexts> Instances;
  }
}
