;; Vect - A simple Vector-Graphic editor for Retriever
;; Copyright (C) 2019 Ingo Ruhnke <grumbel@gmail.com>
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

(use-modules (guix build-system cmake)
             (guix gexp)
             (guix git-download)
             (guix licenses)
             (guix packages)
             (gnu packages game-development)
             (gnu packages pkg-config))

(define %source-dir (dirname (current-filename)))

(define-public vect
  (package
   (name "vect")
   (version "0.4.0")
   (source (local-file %source-dir
                       #:recursive? #t
                       #:select? (git-predicate %source-dir)))
   (build-system cmake-build-system)
   (inputs `(("allegro-4" ,allegro-4)))
   (native-inputs `(("pkg-config" ,pkg-config)))
   (synopsis "A simple vector graphics editor for Allegro/DOS")
   (description "Vect is a simple vector graphics editor coded back in
1998 using Allegro/DOS.  As of 2015, it still compiles under Linux with
only a few minor tweaks.  It's not exactly very feature rich, but it is
able to produce some graphics.  Examples are provided in the
`graphics/` directory.")
   (home-page "https://gitlab.com/grumbel/vect")
   (license gpl3+)))

vect

;; EOF ;;
