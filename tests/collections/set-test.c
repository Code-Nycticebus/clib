#include "clib/collection/set.h"

#include "clib/collection/da.h"
#include "clib/core/assert.h"
#include "clib/type/integer.h"
#include "clib/type/string.h"

#define TEST_SET_DEFAULT_SIZE 10

static void test_set_insert(void) {
  Arena arena = {0};
  Set set = set_create(&arena);

  set_reserve(&set, TEST_SET_DEFAULT_SIZE + 1);
  clib_assert(set.cap == 16, "Did not increase cap the correct way");

  for (usize i = 0; i < TEST_SET_DEFAULT_SIZE * 2; i++) {
    set_add(&set, i);
  }

  clib_assert(set_contains(&set, 0) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 1) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 2) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 3) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 4) == true, "Set should contain this number!");
  clib_assert(set_contains(&set, 5) == true, "Set should contain this number!");

  arena_free(&arena);
}

static void test_set_remove(void) {
  Arena arena = {0};

  Set set = set_create(&arena);

  for (usize i = 0; i < TEST_SET_DEFAULT_SIZE; ++i) {
    set_add(&set, usize_hash(i));
  }

  set_remove(&set, usize_hash(1));
  clib_assert(set_add(&set, usize_hash(5)) == false,
              "5 should already be in set");

  clib_assert(set_remove(&set, usize_hash(5)) == true, "Should remove 5");
  clib_assert(set_contains(&set, usize_hash(5)) == false,
              "Set should not contain 5");

  arena_free(&arena);
}

static void test_set_extend(void) {
  Arena arena = {0};

  const u64 n1[] = {0, 1, 2, 3, 4, 5};
  const u64 n2[] = {6, 7, 8, 9, 10, 11};

  Set set = set_create(&arena);
  set_extend(&set, ARRAY_LEN(n1), n1);
  set_extend(&set, ARRAY_LEN(n2), n2);

  clib_assert(set.cap == 16, "Did not increase size the proper way");

  arena_free(&arena);
}

static void test_set_update(void) {
  Arena arena = {0};
  Set s1 = set_create(&arena);
  const u64 s1_content[] = {1, 2, 3};
  set_extend(&s1, 3, s1_content);

  Set s2 = set_create(&arena);
  const u64 s2_content[] = {4, 5, 6};
  set_extend(&s2, 3, s2_content);

  set_update(&s1, &s2);

  clib_assert(set_contains(&s1, 1), "set should contain 1");
  clib_assert(set_contains(&s1, 2), "set should contain 2");
  clib_assert(set_contains(&s1, 3), "set should contain 3");
  clib_assert(set_contains(&s1, 4), "set should contain 4");
  clib_assert(set_contains(&s1, 5), "set should contain 5");
  clib_assert(set_contains(&s1, 6), "set should contain 6");

  arena_free(&arena);
}

static void test_eq(void) {
  Arena arena = {0};

  Set set1 = set_create(&arena);
  set_add(&set1, 1);
  set_add(&set1, 2);
  set_add(&set1, 3);

  Set set2 = set_create(&arena);
  set_add(&set2, 1);
  set_add(&set2, 2);
  set_add(&set2, 3);

  Set set3 = set_create(&arena);
  set_add(&set3, 2);
  set_add(&set3, 3);
  set_add(&set3, 4);

  clib_assert(set_eq(&set1, &set2) == true, "'set1' should be equal to 'set2'");

  clib_assert(set_eq(&set1, &set3) == false,
              "'set1' should not be equal to 'set3'");
  clib_assert(set_eq(&set2, &set3) == false,
              "'set2' should not be equal to 'set2'");

  arena_free(&arena);
}

static void test_subset(void) {
  Arena arena = {0};

  const u64 n1[] = {1, 2, 3};
  Set set1 = set_create(&arena);
  set_extend(&set1, 3, n1);

  const u64 n2[] = {2, 3, 4};
  Set set2 = set_create(&arena);
  set_extend(&set2, 3, n2);

  const u64 n3[] = {1, 2, 3, 4};
  Set set3 = set_create(&arena);
  set_extend(&set3, 4, n3);

  clib_assert(set_subset(&set1, &set2) == false,
              "'set1' should not be a subset of 'set2'");
  clib_assert(set_subset(&set1, &set1) == true,
              "'set1' should be a subset of 'set1'");
  clib_assert(set_subset(&set1, &set3) == true,
              "'set1' should be a subset of 'set3'");
  clib_assert(set_subset(&set2, &set3) == true,
              "'set2' should be a subset of 'set3'");
  arena_free(&arena);
}

static void test_set_disjoint(void) {
  Arena arena = {0};

  const u64 test_numbers[] = {2, 3, 7, 8};
  const usize count = ARRAY_LEN(test_numbers);
  Set set1 = set_create(&arena);
  set_extend(&set1, count, test_numbers);

  const u64 test_numbers2[] = {2, 8, 9};
  const usize count2 = ARRAY_LEN(test_numbers2);
  Set set2 = set_create(&arena);
  set_extend(&set2, count2, test_numbers2);

  const u64 test_numbers3[] = {4, 5, 6};
  const usize count3 = ARRAY_LEN(test_numbers3);
  Set set3 = set_create(&arena);
  set_extend(&set3, count3, test_numbers3);

  clib_assert(set_disjoint(&set1, &set2) == false, "Should not be disjoint");
  clib_assert(set_disjoint(&set2, &set1) == false, "Should not be disjoint");
  clib_assert(set_disjoint(&set3, &set2) == true, "Should be disjoint");
  clib_assert(set_disjoint(&set3, &set1) == true, "Should be disjoint");

  arena_free(&arena);
}

static void test_intersection(void) {
  Arena arena = {0};

  const u64 test_numbers[] = {2, 3, 7, 8};
  const usize count = sizeof(test_numbers) / sizeof(test_numbers[0]);

  Set set1 = set_create(&arena);
  set_extend(&set1, count, test_numbers);

  const u64 test_numbers2[] = {2, 3, 7, 8, 9};
  const usize count2 = sizeof(test_numbers2) / sizeof(test_numbers2[0]);

  Set set2 = set_create(&arena);
  set_extend(&set2, count2, test_numbers2);

  Set inter = set_intersection(&set1, &set2, &arena);

  clib_assert(set_subset(&inter, &set2) == true, "inter should be a subset");

  arena_free(&arena);
}

static void test_difference(void) {
  Arena arena = {0};

  const u64 test_numbers[] = {1, 2, 3};
  const usize count = sizeof(test_numbers) / sizeof(test_numbers[0]);

  Set set1 = set_create(&arena);
  set_extend(&set1, count, test_numbers);

  const u64 test_numbers2[] = {3, 4, 5};
  const usize count2 = sizeof(test_numbers2) / sizeof(test_numbers2[0]);

  Set set2 = set_create(&arena);
  set_extend(&set2, count2, test_numbers2);

  Set diff = set_difference(&set1, &set2, &arena);

  clib_assert(set_contains(&diff, 1) == true, "should be different");
  clib_assert(set_contains(&diff, 2) == true, "should be different");
  clib_assert(set_contains(&diff, 3) == false, "should not be different");
  clib_assert(set_contains(&diff, 4) == false, "should not be symetrical");
  clib_assert(set_contains(&diff, 5) == false, "should not be symetrical");

  arena_free(&arena);
}

static void test_set_union(void) {
  Arena arena = {0};

  const u64 test_numbers[] = {1, 2, 3};
  const usize count = ARRAY_LEN(test_numbers);
  Set set1 = set_create(&arena);
  set_extend(&set1, count, test_numbers);

  const u64 test_numbers2[] = {3, 4, 5};
  const usize count2 = ARRAY_LEN(test_numbers);
  Set set2 = set_create(&arena);
  set_extend(&set2, count2, test_numbers2);

  Set _unique = set_union(&set1, &set2, &arena);

  clib_assert(set_contains(&_unique, 1) == true, "Set should contain 1");
  clib_assert(set_contains(&_unique, 2) == true, "Set should contain 2");
  clib_assert(set_contains(&_unique, 3) == false, "Set should not contain 3");
  clib_assert(set_contains(&_unique, 4) == true, "Set should contain 4");
  clib_assert(set_contains(&_unique, 5) == true, "Set should contain 6");

  arena_free(&arena);
}

static bool filter_duplicates(void *set, Str s) {
  return set_add(set, str_hash(s));
}

static void test_example_deduplicate(void) {
  Arena arena = {0};

  const Str strings[5] = {
      STR("Apple"), STR("Banana"), STR("Apple"), STR("Cherry"), STR("Apple"),
  };
  DA(Str) list = {0};
  da_init_list(&list, &arena, ARRAY_LEN(strings), strings);

  Arena temp = {0};
  Set unique = set_create(&temp);
  da_filter_ctx(&list, &list, filter_duplicates, &unique);
  arena_free(&temp);

  clib_assert(list.len == 3, "Did not find the unique strings correctly: %" USIZE_FMT,
              list.len);
  clib_assert(str_eq(list.items[0], STR("Apple")),
              "Did not filter out correctly");
  clib_assert(str_eq(list.items[1], STR("Banana")),
              "Did not filter out correctly");
  clib_assert(str_eq(list.items[2], STR("Cherry")),
              "Did not filter out correctly");

  arena_free(&arena);
}

static bool filter_with_set(Set *intersection, Str s) {
  return set_contains(intersection, str_hash(s));
}

static void test_example_duplicates(void) {
  Arena arena = {0};

  const Str strings[5] = {
      STR("Apple"), STR("Banana"), STR("Apple"), STR("Cherry"), STR("Apple"),
  };
  DA(Str) list = {0};
  da_init_list(&list, &arena, ARRAY_LEN(strings), strings);

  Set unique = set_create(&arena);
  Set duplicates = set_create(&arena);

  for (usize i = 0; i < list.len; i++) {
    u64 hash = str_hash(list.items[i]);
    if (set_add(&unique, hash) == false) {
      set_add(&duplicates, hash);
    }
  }

  DA(Str) unique_strings = {0};
  da_init(&unique_strings, &arena);
  da_filter_ctx(&list, &unique_strings, !filter_with_set, &duplicates);

  clib_assert(unique_strings.len == 2,
              "Did not find the unique strings correctly");
  clib_assert(str_eq(unique_strings.items[0], STR("Banana")),
              "Did not filter out correctly");
  clib_assert(str_eq(unique_strings.items[1], STR("Cherry")),
              "Did not filter out correctly");

  arena_free(&arena);
}

static void test_example_intersection(void) {
  Arena arena = {0};

  const Str strings1[7] = {
      STR("Apple"), STR("Banana"), STR("Cherry"), STR("Pomegrade"),
      STR("Grape"), STR("Orange"), STR("Pear"),
  };
  DA(Str) first = {0};
  da_init_list(&first, &arena, ARRAY_LEN(strings1), strings1);

  Set s1 = set_create(&arena);
  for (size_t i = 0; i < first.len; i++) {
    set_add(&s1, str_hash(first.items[i]));
  }

  const Str strings2[7] = {
      STR("Apple"),  STR("Strawberry"), STR("Blueberry"), STR("Banana"),
      STR("Cherry"), STR("Lemon"),      STR("Pear"),
  };
  const usize count2 = ARRAY_LEN(strings2);
  Set s2 = set_create(&arena);
  for (size_t i = 0; i < count2; i++) {
    set_add(&s2, str_hash(strings2[i]));
  }

  Set intersection = set_intersection(&s1, &s2, &arena);
  DA(Str) common = {0};
  da_init(&common, &arena);
  da_filter_ctx(&first, &common, filter_with_set, &intersection);

  clib_assert(common.len == 4,
              "Did not find the intersection correctly: %" USIZE_FMT,
              common.len);
  clib_assert(str_eq(common.items[0], STR("Apple")),
              "Did not filter out correctly");
  clib_assert(str_eq(common.items[1], STR("Banana")),
              "Did not filter out correctly");
  clib_assert(str_eq(common.items[2], STR("Cherry")),
              "Did not filter out correctly");
  clib_assert(str_eq(common.items[3], STR("Pear")),
              "Did not filter out correctly");

  arena_free(&arena);
}

int main(void) {
  test_set_insert();
  test_set_remove();
  test_set_extend();
  test_set_update();
  test_eq();
  test_subset();
  test_set_disjoint();
  test_intersection();
  test_difference();
  test_set_union();

  test_example_deduplicate();
  test_example_duplicates();
  test_example_intersection();
}
