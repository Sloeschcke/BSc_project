# Abstract
Uncertain graphs are widely used for representing the inherent uncertainty in complex systems.
This project reviews and extends the work of Jin et al., by studying the problem of discover-
ing the k most reliable subgraphs in uncertain graphs. This problem is relevant in the analysis
of many network applications, such as social networks, network routing, and protein-protein
interaction networks. As the problem is #P-complete, and thus computationally intractable, a
sampling scheme is introduced to provide ε-approximations with high probability. In doing
this, the problem is transformed to the problem of discovering the k most frequent cohesive
sets in the setting of deterministic graphs. This transformation gives way to an eager and atten-
tive approach. The eager approach combines peeling techniques and an iterative version of the
Apriori algorithm to give approximate solutions with certain ε-guarantees. To ensure that the
correct top-k results are found with high probability, the attentive approach builds on top of
the eager approach by adding a progressive sampling step. The eager and attentive approaches
allow for a trade-off between efficiency and precision. Extensive experiments verify that the
novel approaches provide significant improvements compared to naively extending the work
of Jin et al. to the top-k setting.


Names of algorithms does not correspond to the names in the report.

The equvalences are as follows:
TopKPeeling = AttentivePeeling,
TopKSingleStep = EagerPeeling,
naiveTopKPeeling = NaivePeeling
