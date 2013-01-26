#include "base.h"

#include "config.h"
#include "io.h"
#include "config.h"
#include "hashtable.h"
#include "lexicon.h"
#include "tagset.h"
#include "lbfgs.h"
#include "crf/features.h"
#include "crf/tagger.h"

namespace NLP { namespace CRF {

Tagger::FeatureTypes::FeatureTypes(void)
  : config::OpGroup("types", "feature types config"),
    use_words(*this, "words", "use word features", new WordGen(Types::words)),
    use_prev_words(*this, "prev_words", "use previous word features", new PrevWordGen(Types::words)),
    use_next_words(*this, "next_words", "use next word features", new NextWordGen(Types::words)),
    actives() { }

void Tagger::FeatureTypes::get_tagpair(TagSet &tags, Raws &raws, TagPair &tp, int i) {
  if (i == 0) {
    tp.prev = Tag(Sentinel::val);
    tp.curr = tags.canonize(raws[0]);
  }
  else {
    tp.prev = tags.canonize(raws[i-1]);
    tp.curr = tags.canonize(raws[i]);
  }
}

void Tagger::FeatureTypes::generate(Attributes &attributes, TagSet &tags,
    Sentence &sent, Contexts &contexts, Raws &raws, const bool extract) {
  for(int i = 0; i < sent.words.size(); ++i) {
    for (Actives::iterator it = actives.begin(); it != actives.end(); ++it) {
      OpType *op = *it;
      if ((*op)()) {
        TagPair tp;
        get_tagpair(tags, raws, tp, i);
        if (extract)
          op->generate(attributes, sent, tp, i);
        else {
          contexts[i].klasses = tp;
          contexts[i].index = i;
          op->generate(attributes, sent, contexts[i], i);
        }
      }
    }
  }
}

void Tagger::FeatureTypes::reg(const Type &type, FeatureDict &dict) {
  for (Actives::iterator it = actives.begin(); it != actives.end(); ++it) {
    if ((*it)->has_type(type)) {
      (*it)->reg(&dict);
      return;
    }
  }
}

void Tagger::FeatureTypes::validate(void) {
  for (std::vector<OptionBase *>::iterator child = _children.begin(); child != _children.end(); ++child) {
    (*child)->validate();
    OpType *op = reinterpret_cast<OpType *>(*child);
    if ((*op)())
      actives.push_back(op);
  }
}

} }