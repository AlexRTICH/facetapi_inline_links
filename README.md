# Facets API Inline Links
A Facet API widget that displays facet items as inline links.

Drupal 7.x

## Description

Facet API Inline Links — sub-module (widget) for Facet API, which allows you to organize the output facet elements (reference filter) in one line, that is, using a wrapper ``DIV > SPAN``, instead of ``UL > LI`` (both in standard widgets). Also, facet's counter has been imposed from tag ``A``.

## Install

Setting standard: download, upload module's folder on your hosting.
Next, go to facet's display settings (``./admin/config/search/search_api/index/[MY_SEARCH_INDEX]/facets``) and select «Show Widget». In open select-box choose «Inline links». Thats it!

## Required modules

Facet API ([https://www.drupal.org/project/facetapi](https://www.drupal.org/project/facetapi))

## Special thanks

* This patch very help me: [https://www.drupal.org/files/1367612.9-facetapi-links-render-array.patch](https://www.drupal.org/files/1367612.9-facetapi-links-render-array.patch)
* This module sponsored by [IACM](http://iacm.ru)
