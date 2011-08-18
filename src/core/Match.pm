my class Match is Capture {
    has $.orig;
    has $.from;
    has $.to;
    has $.CURSOR;

    multi method gist(Match:D:) {
        $!to > $!from ?? $!orig.substr($!from, $!to-$!from) !! ''
    }
    multi method Str(Match:D:) {
        $!to > $!from ?? $!orig.substr($!from, $!to-$!from) !! ''
    }
    multi method Numeric(Match:D:) {
        self.Str.Numeric
    }
    multi method Bool(Match:D:) {
        $!to >= $!from
    }
    multi method ACCEPTS(Match:D: Any $) { self }

    method caps(Match:D:) {
        my @caps;
        for self.pairs -> $p {
            if $p.value ~~ Parcel {
                @caps.push: $p.key => $_ for $p.value.list
            } else {
                @caps.push: $p;
            }
        }
        @caps.sort: -> $p { $p.value.from }
    }

    method chunks(Match:D:) {
        my $prev = $!from;
        gather {
            for self.caps {
                if .value.from > $prev {
                    take '~' => $!orig.substr($prev, .value.from - $prev)
                }
                take $_;
                $prev = .value.to;
            }
            take '~' => $!orig.substr($prev, $!to - $prev) if $prev < $!to;
        }
    }
}
