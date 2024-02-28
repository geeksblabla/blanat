<?php

gc_disable();
pcntl_async_signals(true);

function handle_portion($filename, $offset, $end, $pipe) {
  $file = fopen($filename, 'r');
  fseek($file, $offset);
  $cities = [];
  while($offset < $end && ($line = fgets($file)) !== false) {
    $offset += strlen($line);
    list($city, $product, $price) = explode(',', $line);
    $price = (double) $price;
    if (! isset($cities[$city])) $cities[$city] = [0, []];
    $data = &$cities[$city];
    $data[0] += $price;
    if (! isset($data[1][$product])) $data[1][$product] = 101;
    $data[1][$product] = $data[1][$product] < $price ? $data[1][$product] : $price;
  }
  fclose($file);
  fwrite($pipe, json_encode($cities));
  fclose($pipe);
}

$input_path = 'input.txt';
$pids = [];
$pipes = [];
$results = [];

$file_size = filesize($input_path);
$part_size = (int) ($file_size / 8);

$file = fopen($input_path, 'r');
$pos = 0;
for ($i = 0; $i < 8; $i++) {
  $offset = $pos;
  $pos += $part_size;
  fseek($file, $pos);
  $line = fgets($file);
  if ($line !== false) $pos += strlen($line);
  if ($pos > $file_size) $pos = $file_size;
  $pipe = stream_socket_pair(STREAM_PF_UNIX, STREAM_SOCK_STREAM, STREAM_IPPROTO_IP);
  $pid = pcntl_fork();
  if ($pid == -1) die('Could not fork!');
  if ($pid) {
    fclose($pipe[0]);
    $pipes[$pid] = $pipe[1];
    $pids[] = $pid;
  } else {
    fclose($pipe[1]);
    handle_portion($input_path, $offset, $pos, $pipe[0]);
    exit(0);
  }
}
fclose($file);

foreach ($pids as $child_pid) {
  pcntl_waitpid($child_pid, $status);
  $results[] = json_decode(fgets($pipes[$child_pid]), true);
  fclose($pipes[$child_pid]);
}

$totals = [];
foreach ($results as $res) {
  foreach ($res as $city => $data) {
    if (!isset($totals[$city])) $totals[$city] = 0;
    $totals[$city] += $data[0];
  }
}

$cheapest_city = array_key_first($totals);
$cheapest_total = $totals[$cheapest_city];
foreach ($totals as $city => $total) {
  if ($total < $cheapest_total) {
    $cheapest_city = $city;
    $cheapest_total = $total;
  }
}

$products = [];
foreach ($results as $res) {
  if (!isset($res[$cheapest_city])) continue;
  foreach ($res[$cheapest_city][1] as $product => $price) {
    if (!isset($products[$product]) || $products[$product] > $price) {
      $products[$product] = $price;
    }
  }
}
uksort($products, function($p1, $p2) use(&$products) {
  $v1 = $products[$p1];
  $v2 = $products[$p2];
  if ($v1 === $v2) {
    if ($p1 === $p2) return 0;
    return $p1 < $p2 ? -1 : 1;
  }
  if ($v1 === $v2) return 0;
  return $v1 < $v2 ? -1 : 1;
});
$products = array_slice($products, 0, 5, true);

$file = fopen('output.txt', 'w');
fprintf($file, "%s %.2f\n", $cheapest_city, $cheapest_total);

foreach($products as $product => $price) {
  fprintf($file, "%s %.2f\n", $product, $price);
}
fclose($file);
