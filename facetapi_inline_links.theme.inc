<?php

/**
 * @file
 * Theme functions for the Facet API Inline Links module.
 */

/**
 * Returns HTML for a list or nested list of items.
 *
 * An almost exact copy of theme_item_list, except is uses renderable arrays properly.
 * See http://drupal.org/node/891112  Once that issue is resolved in core, this function
 * can go away.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An array of items to be displayed in the list. If an item is a
 *     string, then it is used as is. If an item is an array, then the "data"
 *     element of the array is used as the contents of the list item. If an item
 *     is an array with a "children" element, those children are displayed in a
 *     nested list. All other elements are treated as attributes of the list
 *     item element.
 *   - title: The title of the list.
 *   - type: The type of list to return (e.g. "ul", "ol").
 *   - attributes: The attributes applied to the list element.
 */
function theme_facetapi_inline_links_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];
  
  $output = '';
  if (isset($title) && $title !== '') {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= '<div' . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $i = 0;
    foreach ($items as $item) {
      $attributes = array();
      $children = array();
      $data = '';
      $i++;
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
	  $attributes['class'][] = 'inline-link';
      $output .= '<span' . drupal_attributes($attributes) . '>' . $data . "</span>\n";
    }
    $output .= "</div>";
  }
  
  return $output;
}

/**
 * Returns HTML for a "missing" facet link.
 *
 * @param $variables
 *   An associative array containing:
 *   - field_name: The name of the facet field.
 *
 * @ingroup themeable
 */
function theme_facetapi_inline_links_facet_missing($variables) {
  return t('Missing %field_name', array('%field_name' => $variables['field_name']));
}

/**
 * Returns HTML for the facet title, usually the title of the block.
 *
 * @param $variables
 *   An associative array containing:
 *   - title: The title of the facet.
 *   - facet: The facet definition as returned by facetapi_facet_load().
 *
 * @ingroup themeable
 */
function theme_facetapi_inline_links_title($variables) {
  return t('Filter by @title:', array('@title' => drupal_strtolower($variables['title'])));
}

/**
 * Returns HTML for an inactive facet item.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'path', 'options', and
 *   'count'. See the l() and theme_facetapi_count() functions for information
 *   about these variables.
 *
 * @ingroup themeable
 */
function theme_facetapi_inline_links_link_inactive($variables) {
  // @see https://www.drupal.org/node/1367612#comment-5351740
  if (isset($variables[''])) {
    $variables = $variables[''];
  }
  
  $accessible_variables = array(
    'text' => $variables['text'],
    'active' => FALSE,
  );
  $accessible_markup = theme('facetapi_accessible_markup', $accessible_variables);

  // Sanitizes the link text if necessary.
  $sanitize = empty($variables['options']['html']);
  $variables['text'] = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Resets link text, sets to options to HTML since we already sanitized the
  // link text and are providing additional markup for accessibility.
  $variables['text'] .= $accessible_markup;
  $variables['options']['html'] = TRUE;

  // Adds count to link if one was passed.
  if (isset($variables['count'])) {
    return theme_link($variables) . '&nbsp;' . theme('facetapi_count', array('count' => $variables['count']));
  }
  else {
    return theme_link($variables);
  }
}

/**
 * Returns HTML for the active facet item's count.
 *
 * @param $variables
 *   An associative array containing:
 *   - count: The item's facet count.
 *
 * @ingroup themeable
 */
function theme_facetapi_inline_links_count($variables) {
  return '(' . (int) $variables['count'] . ')';
}

/**
 * Returns HTML for an active facet item.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'path', and 'options'. See
 *   the l() function for information about these variables.
 *
 * @see l()
 *
 * @ingroup themeable
 */
function theme_facetapi_inline_links_link_active($variables) {
  // @see https://www.drupal.org/node/1367612#comment-5351740
  if (isset($variables[''])) {
    $variables = $variables[''];
  }
  
  // Sanitizes the link text if necessary.
  $sanitize = empty($variables['options']['html']);
  $link_text = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Theme function variables fro accessible markup.
  // @see http://drupal.org/node/1316580
  $accessible_vars = array(
    'text' => $variables['text'],
    'active' => TRUE,
  );

  // Builds link, passes through t() which gives us the ability to change the
  // position of the widget on a per-language basis.
  $replacements = array(
    '!facetapi_deactivate_widget' => theme('facetapi_deactivate_widget', $variables),
    '!facetapi_accessible_markup' => theme('facetapi_accessible_markup', $accessible_vars),
  );
  $variables['text'] = t('!facetapi_deactivate_widget !facetapi_accessible_markup', $replacements);
  $variables['options']['html'] = TRUE;
  return theme_link($variables) . $link_text;
}

/**
 * Returns HTML for the deactivation widget.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'path', and 'options'. See
 *   the l() function for information about these variables.
 *
 * @see l()
 * @see theme_facetapi_link_active()
 *
 * @ingroup themable
 */
function theme_facetapi_inline_links_deactivate_widget($variables) {
  return '(' . t('Cancel') . ')';
}

/**
 * Returns HTML that adds accessible markup to facet links.
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The text of the facet link.
 *   - active: Whether the item is active or not.
 *
 * @ingroup themeable
 *
 * @see http://drupal.org/node/1316580
 */
function theme_facetapi_inline_links_accessible_markup($variables) {
  $vars = array('@text' => $variables['text']);
  $text = ($variables['active']) ? t('Remove @text filter', $vars) : t('Apply @text filter', $vars);
  // Add spaces before and after the text, since other content may be displayed
  // inline with this and we don't want the words to run together. However, the
  // spaces must be inside the <span> so as not to disrupt the layout for
  // sighted users.
  return '<span class="element-invisible"> ' . $text . ' </span>';
}
